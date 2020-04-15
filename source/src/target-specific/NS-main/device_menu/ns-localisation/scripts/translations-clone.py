#!/usr/bin/env python

import poeditor
import argparse


def main():
    parser = argparse.ArgumentParser(
        description='Clones POEditor translations from one language to all others in a project.')
    parser.add_argument('api_token', help='POEditor API token https://poeditor.com/account/api')
    parser.add_argument('project_id', help='POEditor project id https://poeditor.com/account/api')

    parser.add_argument('--language-to-clone',
                        help='Clone translations from this language. Defaults to \'en\'.',
                        default='en', metavar='LANG_CODE')
    parser.add_argument('-t', '--tags',
                        help='Only translations of terms matching this set of tags will be cloned. '
                             'Defaults to \'do-not-translate\'.',
                        metavar='TAG', nargs='+', default=['do-not-translate'])

    args = parser.parse_args()

    lang_code_to_clone = args.language_to_clone
    lang_region_codes = poeditor.fetch_language_codes(args.api_token, args.project_id)
    lang_region_codes.remove(lang_code_to_clone)

    tags_to_clone = args.tags

    translations_to_clone = poeditor.fetch_translations_json(args.api_token, args.project_id,
                                                             lang_code_to_clone, tags_to_clone)

    if not tags_to_clone or tags_to_clone == ['']:
        print 'Found {0} terms'.format(len(translations_to_clone))
    else:
        print 'Found {0} terms with tags matching all of {1}'.format(len(translations_to_clone),
                                                                     tags_to_clone)

    if 0 == len(translations_to_clone):
        print 'Nothing to clone'
        return

    print 'Cloning from {0} to {1} languages: {2}'.format(lang_code_to_clone,
                                                          len(lang_region_codes), lang_region_codes)

    for lang_region_code in lang_region_codes:
        success, result = poeditor.upload_translations_json(args.api_token, args.project_id,
                                                            lang_region_code, translations_to_clone)
        if success:
            print 'Cloned {0} translations to {1}'.format(lang_code_to_clone, lang_region_code)
            print result
        else:
            print 'Failed to clone {0} translations to {1}: {2}'.format(lang_code_to_clone,
                                                                        lang_region_code, result)


if __name__ == "__main__":
    main()
