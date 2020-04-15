#ifndef LOCALIZATION_KEYS_H
#define LOCALIZATION_KEYS_H

#include "keys.h"
#include <string>

namespace localization {

void setup();

const Language &currentLanguage();
Language nextLanguage(const Language &language);
Language prevLanguage(const Language &language);
void setLanguage(const Language &language);
void saveLanguage(void);
std::string languageName(const Language &language);

std::string localizedString(const LocalizationKey &key);

} // namespace localization

#define LocalizedString(x) localization::localizedString(x)

#endif
