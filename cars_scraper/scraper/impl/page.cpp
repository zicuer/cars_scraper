#include "stdafx.h"
#include "page.h"
#include "page_moc.h"
#include "manager.h"

namespace scraper {
namespace impl
{
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
   QWebFrame const* page_t::frame () const
   {
      return page_->mainFrame();
   }
}}