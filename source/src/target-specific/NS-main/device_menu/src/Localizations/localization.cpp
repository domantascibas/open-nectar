#include "localization.h"
#include "Storage.h"

#include "en.h"
#include "lt.h"
#include "ru.h"
#include "de.h"
#include "stdint.h"

namespace localization {

static const std::string error = "Language not implemented";

Language _language = LanguageCout;

void setup() { setLanguage(EN); };

const Language &currentLanguage() { return _language; }

Language nextLanguage(const Language &language) {
  int l = language + 1;
  if (l >= LanguageCout)
    l = 0;
  return (Language)l;
};

Language prevLanguage(const Language &language) {
  int l = language - 1;
  if (l < 0)
    l = LanguageCout - 1;
  return (Language)l;
};

std::string languageName(const Language &language) {
  switch (language) {
  case EN:
    return "EN";
  case LT:
    return "LT";
  case RU:
    return "RU";
  case DE:
    return "DE";
  default:
    return error;
  }
};

void setLanguage(const Language &language) {
  printf("[LANGUAGE] selected language %d\n", language);
  if (_language == language)
    return;
  _language = language;
//  Storage::saveLanguage(language);
};

void saveLanguage(void) {
	Storage::saveLanguage(currentLanguage());
};

std::string localizedString(const LocalizationKey &key) {
  switch (_language) {
  case EN:
    return en_string(key);
  case LT:
    return lt_string(key);
  case RU:
    return ru_string(key);
  case DE:
    return de_string(key);
  default:
    return error;
  }
};

} // namespace localization
