#!/usr/bin/env python
# DEPRECATED
# See & use translations-download.py instead
#
# Fetches TRAFI mobile localizations from POEditor.
# Usage: python localizations-update.py localization-config.json
#

import httplib
import urllib
import json
import os
import sys
from collections import defaultdict
import io_utils
import platform_utils


# downloads list of translated languages
# https://poeditor.com/docs/api#languages_list
def fetch_language_codes(api, token, project_id):
    headers = {
        "Content-type": "application/x-www-form-urlencoded",
        "Accept": "application/json"
    }
    params = urllib.urlencode({
        'api_token': token,
        'id': project_id,
    })
    api.request("POST", "/v2/languages/list", params, headers)
    languages = json.loads(api.getresponse().read())['result']['languages']
    codes = []
    for language in languages:
        codes.append(language['code'])
    return codes


# exports and downloads translations in the given type for a given language
# https://poeditor.com/docs/api#projects_export
def fetch_strings(api, token, project_id, language_code, type):
    headers = {
        "Content-type": "application/x-www-form-urlencoded",
        "Accept": "application/json"
    }
    params = urllib.urlencode({
        'api_token': token,
        'id': project_id,
        'language': language_code,
        'type': type
    })
    api.request("POST", "/v2/projects/export", params, headers)
    url = json.loads(api.getresponse().read())['result']['url']
    return urllib.urlopen(url).read()


def main(argv):
    if len(argv) < 2:
        print 'Please specify a config file'
        return

    config_filename = argv[1]
    if not os.path.isfile(config_filename):
        print 'Please specify a valid config file'
        return

    config_data = open(config_filename, 'r').read()
    config = json.loads(config_data)

    project_id = config.get('project_id')
    if not project_id:
        print 'Please specify a project_id with the POEditor project id'
        return

    api_token = config.get('api_token')
    if not api_token:
        print 'Please specify an api_token for the POEditor API'
        return

    platform = config.get('platform')
    if platform not in ['mobile', 'mbed', 'web']:
        print 'Please specify platform as either \'mobile\' or \'mbed\' or \'web\''
        return

    print 'Updating localizations for {0}..'.format(platform)

    # treat output_dir as relative to config file dir
    abs_config_dir = os.path.dirname(os.path.abspath(config_filename))
    output_dir = os.path.join(abs_config_dir, config.get('output_dir', platform))

    api = httplib.HTTPSConnection("api.poeditor.com")
    lang_region_codes = config.get('languages')
    if not lang_region_codes:
        # if not specified, fetch all available languages
        lang_region_codes = fetch_language_codes(api, api_token, project_id)

    print 'Fetching {0} languages: {1}'.format(len(lang_region_codes), lang_region_codes)

    lang_to_output_files = defaultdict(list)
    export_type = 'json'
    if 'mobile' == platform:
        output_path_template = os.path.join(output_dir, '{0}.js')
        map_data = platform_utils.map_mobile_data
    elif 'mbed' == platform:
        output_path_template = os.path.join(output_dir, '{0}.h')
        map_data = platform_utils.map_mbed_data
    elif 'web' == platform:
        output_path_template = os.path.join(output_dir, '{0}.yml')
        map_data = platform_utils.map_web_data
        export_type = 'key_value_json'
    else:
        raise Exception('Unknown platform {0}'.format(platform))

    for lang_region_code in lang_region_codes:
        lang_code = platform_utils.strip_region(lang_region_code)

        output_file = output_path_template.format(lang_code, lang_code.upper())
        lang_to_output_files[lang_region_code].append(output_file)

    for lang_region_code, output_files in lang_to_output_files.iteritems():
        try:
            print 'Downloading language {0}'.format(lang_region_code)
            data = fetch_strings(api, api_token, project_id, lang_region_code, export_type)
            data = map_data(data, lang_region_code).encode('utf-8')

            for output_file in output_files:
                print 'Saving to {0}'.format(output_file)
                io_utils.save_strings(data, output_file)
        except OSError as e:
            print e
            print 'Failed to save localizations for {0}'.format(lang_region_code)

    print 'Localization update complete for {0}'.format(platform)


if __name__ == "__main__":
    main(sys.argv)
