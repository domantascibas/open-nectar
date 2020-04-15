#!/usr/bin/env python

import poeditor
import argparse


def find_by_term_name(terms_or_translations, name):
    for term_or_translation in terms_or_translations:
        if term_or_translation['term'] == name:
            return term_or_translation
    return None


def main():
    parser = argparse.ArgumentParser(
        description='Clones POEditor terms and their associated translations.')
    parser.add_argument('api_token', help='POEditor API token https://poeditor.com/account/api')
    parser.add_argument('project_id', help='POEditor project id https://poeditor.com/account/api')
    parser.add_argument('term_to_clone', help='Name of the term to clone.')
    parser.add_argument('new_term', help='Name of the new term to create with cloned translations.')

    args = parser.parse_args()

    term_to_clone_name = args.term_to_clone
    new_term_name = args.new_term

    terms_json = poeditor.fetch_terms_json(args.api_token, args.project_id)

    term_to_clone = find_by_term_name(terms_json, term_to_clone_name)
    if not term_to_clone:
        print 'Term {0} not found'.format(term_to_clone_name)
        return

    print 'Cloning term {0}'.format(term_to_clone_name)

    new_term = term_to_clone
    new_term['term'] = new_term_name

    terms_to_add = [new_term]
    success, result = poeditor.add_terms_json(args.api_token, args.project_id, terms_to_add)
    if success:
        print 'Created new term {0}'.format(new_term_name)
        print result
    else:
        print 'Failed to create new term {0}'.format(new_term_name)
        return

    print 'Cloning {0} translations'.format(term_to_clone_name)

    lang_codes = poeditor.fetch_language_codes(args.api_token, args.project_id)
    for lang_code in lang_codes:
        all_translations = poeditor.fetch_translations_json(args.api_token, args.project_id,
                                                            lang_code)
        translation = find_by_term_name(all_translations, term_to_clone_name)
        if not translation:
            print 'Did not find {0} translation for {1}'.format(lang_code, term_to_clone_name)
            continue

        translation['term'] = new_term_name
        translations_to_upload = [translation]

        success, result = poeditor.upload_translations_json(args.api_token, args.project_id,
                                                            lang_code, translations_to_upload)
        if success:
            print 'Cloned {0} translation to {1}'.format(lang_code, new_term_name)
            print result
        else:
            print 'Failed to clone {0} {1} translation to {2}'.format(term_to_clone_name, lang_code,
                                                                      new_term_name)


if __name__ == "__main__":
    main()
