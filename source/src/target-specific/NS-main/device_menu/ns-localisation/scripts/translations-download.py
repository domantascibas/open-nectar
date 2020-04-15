#!/usr/bin/env python

import os
import poeditor
from collections import defaultdict
import io_utils
import platform_utils
import argparse


def main():
    parser = argparse.ArgumentParser(
        description='Downloads TRAFI mobile translations from POEditor.')
    parser.add_argument('api_token', help='POEditor API token https://poeditor.com/account/api')
    parser.add_argument('project_id', help='POEditor project id https://poeditor.com/account/api')
    parser.add_argument('platform',
                        help='Translations will be downloaded in the native format for the provided platform.',
                        choices=['android', 'ios'])
    parser.add_argument('-t', '--tags',
                        help='Only terms matching this set of tags will be downloaded. '
                             'Defaults to platform.',
                        metavar='TAG', nargs='+')
    parser.add_argument('-l', '--languages',
                        help='Only translations in the provided set of languages will be downloaded. '
                             'Defaults to all available languages.',
                        metavar='LANG_CODE', nargs='+')
    parser.add_argument('-o', '--output-dir',
                        help='Root output directory for the downloaded translation files. '
                             'Defaults to platform name under the working directory.',
                        metavar='PATH')
    parser.add_argument('--output-template',
                        help='Template to name each downloaded translation file. Optionally, specify placeholders: '
                             '{0} - language code in lower case, '
                             '{1} - language code in upper case; '
                             'Defaults to TRAFI platform default.')

    args = parser.parse_args()

    print 'Downloading translations for {0}..'.format(args.platform)

    tags = args.tags
    if not tags:
        tags = [args.platform]

    print 'Downloading terms with tags matching all of {0}'.format(tags)

    lang_region_codes = args.languages
    if not lang_region_codes:
        # if not specified, fetch all available languages
        lang_region_codes = poeditor.fetch_language_codes(args.api_token, args.project_id)

    print 'Fetching {0} languages: {1}'.format(len(lang_region_codes), lang_region_codes)

    output_dir = args.output_dir or args.platform
    output_template = args.output_template
    lang_to_output_files = defaultdict(list)

    if 'android' == args.platform:
        if not output_template:
            output_template = os.path.join('values-{0}', 'localization-{1}-android.xml')

        output_path_template = os.path.join(output_dir, output_template)
        output_type = 'android_strings'
        map_code = platform_utils.map_android_lang_code
        map_data = platform_utils.map_android_data
        # en is used for default android resources
        if 'en' in lang_region_codes:
            lang_to_output_files['en'].append(
                os.path.join(output_dir, 'values', 'localization-EN-android.xml'))
    elif 'ios' == args.platform:
        if not output_template:
            output_template = os.path.join('{0}.lproj', 'Localizable.strings')

        output_path_template = os.path.join(output_dir, output_template)
        output_type = 'apple_strings'
        map_code = platform_utils.map_ios_lang_code
        map_data = platform_utils.map_ios_data
    else:
        raise Exception('Unknown platform {0}'.format(args.platform))

    for lang_region_code in lang_region_codes:
        lang_code = map_code(lang_region_code)

        output_file = output_path_template.format(lang_code, lang_code.upper())
        lang_to_output_files[lang_region_code].append(output_file)

    for lang_region_code, output_files in lang_to_output_files.iteritems():
        try:
            print 'Downloading language {0}'.format(lang_region_code)
            data = poeditor.fetch_translations(args.api_token, args.project_id, lang_region_code,
                                               output_type, tags)
            data = map_data(data)

            for output_file in output_files:
                print 'Saving to {0}'.format(output_file)
                io_utils.save_strings(data, output_file)
        except:
            print 'Failed to save translations for {0}'.format(lang_region_code)

    print 'Translation download complete for {0}'.format(args.platform)


if __name__ == "__main__":
    main()
