# Localizations

[![POEditor](https://poeditor.com/public/images/logo_small.png)](http://poeditor.com)

## Adding new terms
We're adding new *terms* (strings to translate) directly to [POEditor](http://poeditor.com). All added *terms* must have [assigned *tags*](https://poeditor.com/kb/organizing-strings-with-the-tagging-system) to specify platform (`api|ios|android`). Additionally, provide screenshots for more context when possible - see [POEditor's Knowledge Base](https://poeditor.com/kb/how-to-add-screenshots-to-a-poeditor-localization-project) for more info.

### Single
To add a single new term use POEditor's *View or Add Terms* option in menu:

![](./Img/menu-view-or-add-terms.png =166x)

After adding a new *term* don't forget to assign platform *tags*.
See up to date example in [POEditor's Knowledge Base](https://poeditor.com/kb/how-to-manually-add-delete-terms) for more details.

### Multiple
To add multiple new terms use POEditor's *Terms Import* option in menu:

![](./Img/menu-import-terms.png =166x)

There you can import a file containing the new *terms* and their English translations. CSV file example:
```(csv)
app_name,Nectar Sun
setup_title,Setup
```

After the import you'll see a warning that you should ignore about the terms in the project missing from imported file.

## Add a new contributor
For instructions to add a new contributor see up to date [POEditor's Knowledge Base tutorial](https://poeditor.com/kb/adding-contributors-translators).

## Updating localizations
### Mobile
Run `localizations-update.py` under `scripts`, specifying a valid JSON config file.

**Required** config properties:

| Property     | Description |
| ------------ | ----------- |
| `project_id` | The [POEditor project id][poeditor-api-access] |
| `api_token`  | The [POEditor API token][poeditor-api-access]  |
| `platform`   | `android` or `ios` determines the output file format |

**Optional** config properties:

| Property          | Description |
| ----------------- | ----------- |
| `output_dir`      | Output directory relative to config file, defaults to directory matching name of `platform` |
| `languages`       | Array of language codes to update, e.g. `["en", "id", "lt"]`, defaults to all project languages |
| `tags`            | Array of tags that terms must match, e.g. `["api"]`, defaults to tag matching name of `platform` |

Sample config file: `localization-config.json`
```json
{
  "project_id":"81055",
  "api_token":"ENTER_POEDITOR_API_TOKEN",
  "platform":"ios",
  "output_dir":"output_ios"
}
```

```bash
python localizations-update.py localization-config.json
```

### React-native

To update localizations for react-native project simply use [Fastlane](fastlane/fastlane) from terminal:
```
fastlane Localize
```

### Mbed menus

To update localizations for mbed project, run `python` script for localizations:

```
python localizations-update.py config-mbed.json
```

## Other POEditor questions
See [POEditor's Knowledge Base](https://poeditor.com/kb/) with small, simple and clear tutorials

See [POEditor's FAQ](https://poeditor.com/help/faq) for any other questions.


[poeditor-api-access]: https://poeditor.com/account/api
[trafi-android]: https://github.com/trafi/trafi-android
