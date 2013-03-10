#include "stdafx.h"
#include "offers_table_model.h"
#include "offers_table_model_moc.h"

namespace gui {
namespace impl
{
   namespace
   {
      enum column_t
      {
         column_model = 0,
         column_price,
         column_year,
         column_run,

         column_count
      };

      wstring column_name ( int column_id )
      {
         switch (column_id)
         {
         case column_model:
            return L"Модель";
         case column_price:
            return L"Стоимость";
         case column_year:
            return L"Год выпуска";
         case column_run:
            return L"Пробег";
         }

         assert(false);
         return L"";
      }
   }

   offers_table_model_t::offers_table_model_t ( QObject * parent )
      : QAbstractTableModel ( parent )
   {}

   // QAbstractTableModel
   //////////////////////////////////////////////////////////////////////////
   int offers_table_model_t::rowCount ( QModelIndex const& parent ) const
   {
      assert(!parent.isValid());
      return offers_.size();
   }

   int offers_table_model_t::columnCount ( QModelIndex const& parent ) const
   {
      assert(!parent.isValid());
      return column_count;
   }

   QVariant offers_table_model_t::data ( QModelIndex const& index, int role ) const
   {
      assert(index.isValid());
      assert(!index.parent().isValid());

      assert(index.model() ==
         static_cast<QAbstractItemModel const*>(this));
      assert((index.row() < offers_.size()) &&
             (index.column() < column_count));

      scraper::offer_t const& offer =
         offers_[index.row()];

      switch (role)
      {
      case Qt::DisplayRole:
         {
            switch (index.column())
            {
            case column_model:
               return utils::to_variant(offer.model);
            case column_price:
               return offer.price ? utils::to_variant(*offer.price)
                                  : QVariant();
            case column_year:
               return offer.year ? utils::to_variant(*offer.year)
                                 : QVariant();
            case column_run:
               return offer.run ? utils::to_variant(*offer.run)
                                : QVariant();
            }
         }
         break;

      case Qt::ToolTipRole:
         {
            static wformat phone_fmt (L"+7(%s)%s");

            vector<wstring> phones;
            for each (scraper::phone_number_t const& phone_number in offer.phone_numbers)
               phones.push_back(str(phone_fmt % phone_number.area_code % phone_number.number));

            return utils::to_variant(algorithm::join(phones, L"\n"));
         }
         break;

      case Qt::TextAlignmentRole:
         {
            switch (index.column())
            {
            case column_model:
               return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
            case column_price:
            case column_year:
            case column_run:
               return QVariant(Qt::AlignRight | Qt::AlignVCenter);
            }
         }
         break;
      }

      return QVariant();
   }

   QVariant offers_table_model_t::headerData ( int section,
      Qt::Orientation orientation, int role ) const
   {
      if ((role != Qt::DisplayRole) ||
          (orientation == Qt::Vertical))
         return QVariant();

      return utils::to_variant(column_name(section));
   }

   // impl
   //////////////////////////////////////////////////////////////////////////
   void offers_table_model_t::add_offer ( scraper::offer_t const& offer )
   {
      optional<wstring> const offer_id =
         scraper::get_offer_id(offer.url);

      if (offer_id && offers_ids_.find(*offer_id) == offers_ids_.end())
      {
         beginInsertRows(QModelIndex(), offers_.size(),
                                        offers_.size());
         offers_.push_back(offer);
         offers_ids_.insert(*offer_id);
         endInsertRows();
      }
   }

   scraper::offer_t const&
      offers_table_model_t::get_offer ( int row ) const
   {
      return offers_[row];
   }

   void offers_table_model_t::rem_offers ()
   {
      beginRemoveRows(QModelIndex(), 0, offers_.size() - 1);
      offers_.clear();
      offers_ids_.clear();
      endRemoveRows();
   }

   unsigned offers_table_model_t::offers_count () const
   {
      return offers_.size();
   }
}}