#pragma once

struct settings_t
   : noncopyable
{
   settings_t ();

   template<class T>
      void set_value ( wstring const& key, T const& value );
   template<class T>
      T get_value ( wstring const& key, T const& def ) const;

private:
   QSettings settings_;
};

template<class T>
   void settings_t::set_value ( wstring const& key, T const& val )
{
   settings_.setValue(utils::to_qt(key), utils::to_variant(val));
}

template<class T>
   T settings_t::get_value ( wstring const& key, T const& def ) const
{
   QVariant const val = settings_.value(utils::to_qt(key));
   return val.isNull() ? def : utils::from_variant<T>(val);
}