#include "stdafx.h"
#include "page.h"
#include "page_moc.h"
#include "manager.h"

namespace scraper {
namespace impl
{
   namespace
   {
      vector<wstring> split_query ( wstring const& compressed_query )
      {
         vector<wstring> queries;
         split(queries, compressed_query,
            is_any_of(L"|"));
         return queries;
      }

      optional<wstring> find_any ( QWebElement element,
         vector<wstring>::const_iterator beg,
         vector<wstring>::const_iterator end )
      {
         QWebElementCollection const childs =
            element.findAll(utils::to_qt(*beg++));

         for (QWebElementCollection::const_iterator i = childs.begin();
            i != childs.end(); ++i)
         {
            if (beg == end)
               return utils::from_qt((*i).toPlainText());

            if (optional<wstring> const result = find_any(*i, beg, end))
               return result;
         }
         return none;
      }

      void find_all ( QWebElement element,
         vector<wstring>::const_iterator beg,
         vector<wstring>::const_iterator end,
         vector<wstring> & results )
      {
         QWebElementCollection const childs =
            element.findAll(utils::to_qt(*beg++));

         for (QWebElementCollection::const_iterator i = childs.begin();
            i != childs.end(); ++i)
         {
            if (beg == end)
               results.push_back(utils::from_qt((*i).toPlainText()));
            else
               find_all(*i, beg, end, results);
         }
      }
   }

   page_t::page_t ( QObject * parent )
      : QObject ( parent )
   {
      reset();

      timer_ = new QTimer(this);
      timer_->setSingleShot(true);

      QObject::connect(timer_, SIGNAL(timeout()),
                       this, SIGNAL(timeout()));
   }

   void page_t::reset ()
   {
      filter_t filter;
      if (page_)
      {
         if (is_visible())
            set_visible(false);

         QObject::disconnect(page_, SIGNAL(loadFinished(bool)),
                             this, SLOT(on_loaded(bool)));

         filter = get_filter();
         page_->deleteLater();
      }

      page_ = new QWebPage(this);
      page_->setNetworkAccessManager(new manager_t(page_));

      set_filter(filter);
      QObject::connect(page_, SIGNAL(loadFinished(bool)),
                       this, SIGNAL(loaded(bool)));
   }

   //////////////////////////////////////////////////////////////////////////
   void page_t::load ( QUrl const& url,
                       optional<double> timeout )
   {
      page_->mainFrame()->load(url);
      if (timeout)
         timer_->start(static_cast<int>(*timeout * 1000.));
   }

   void page_t::stop ()
   {
      page_->triggerAction(QWebPage::Stop);
      timer_->stop();
   }

   //////////////////////////////////////////////////////////////////////////
   void page_t::set_visible ( bool visible )
   {
      if (is_visible() == visible)
         return;

      if (visible)
      {
         QWebView * view = new QWebView();
         QObject::connect(view, SIGNAL(destroyed()),
                          this, SIGNAL(canceled()));

         view->setWindowModality(Qt::ApplicationModal);
         view->setAttribute(Qt::WA_DeleteOnClose, true);
         view->setAttribute(Qt::WA_QuitOnClose, false);

         view->setPage(page_);
         view->show();
      }
      else
      {
         QWidget * view = page_->view();
         QObject::disconnect(view, SIGNAL(destroyed()),
                             this, SIGNAL(canceled()));
         view->close();
      }
   }

   bool page_t::is_visible () const
   {
      return (page_->view() != NULL);
   }

   //////////////////////////////////////////////////////////////////////////
   void page_t::set_filter ( filter_t filter )
   {
      dynamic_cast<manager_t *>(page_->networkAccessManager())->set_filter(filter);
   }

   filter_t page_t::get_filter () const
   {
      return dynamic_cast<manager_t const*>(page_->networkAccessManager())->get_filter();
   }

   //////////////////////////////////////////////////////////////////////////
   optional<wstring> page_t::find_any ( wstring const& compressed_query ) const
   {
      vector<wstring> const queries =
         split_query(compressed_query);
      assert(!queries.empty());

      return scraper::impl::find_any(page_->mainFrame()->documentElement(),
         queries.begin(), queries.end());
   }

   vector<wstring> page_t::find_all ( wstring const& compressed_query ) const
   {
      vector<wstring> const queries =
         split_query(compressed_query);
      assert(!queries.empty());

      vector<wstring> results;
      scraper::impl::find_all(page_->mainFrame()->documentElement(),
         queries.begin(), queries.end(), results);

      return results;
   }
}}