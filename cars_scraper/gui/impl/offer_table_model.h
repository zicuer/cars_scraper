#pragma once

#include "scraper/offer.h"

namespace gui {
namespace impl
{
   class offers_table_model_t
      : public QAbstractTableModel
   {
   public:
      explicit offers_table_model_t ( QObject * parent = NULL );

   // QAbstractTableModel
   public:
      int rowCount ( QModelIndex const& parent ) const;
      int columnCount ( QModelIndex const& parent ) const;

      QVariant data ( QModelIndex const& index, int role ) const;
      QVariant headerData ( int section,
         Qt::Orientation orientation, int role ) const;

   public:
      void add_offer ( scraper::offer_t const& offer );
      scraper::offer_t const&
           get_offer ( int row ) const;
      void rem_offers ();

      unsigned offers_count () const;

   private:
      typedef
         vector<scraper::offer_t>
         offers_t;

      typedef
         set<wstring>
         offers_ids_t;

      offers_t offers_;
      offers_ids_t offers_ids_;

   private:
      Q_OBJECT
   };
}}