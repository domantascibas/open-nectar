# POEditor API v2
# https://poeditor.com/docs/api
import requests
import json

# downloads list of translated languages
# https://poeditor.com/docs/api#languages_list
def fetch_language_codes(api_token, project_id):
    headers = {
        'Accept': 'application/json'
    }
    payload = {
        'api_token': api_token,
        'id': project_id,
    }

    r = requests.post('https://api.poeditor.com/v2/languages/list', data=payload, headers=headers)
    languages = r.json()['result']['languages']
    codes = []
    for language in languages:
        codes.append(language['code'])
    return codes

# downloads list of project's terms
# https://poeditor.com/docs/api#terms_list
def fetch_terms_json(api_token, project_id):
    headers = {
        'Accept': 'application/json'
    }
    payload = {
        'api_token': api_token,
        'id': project_id,
        # 'language': language_code,
    }

    r = requests.post('https://api.poeditor.com/v2/terms/list', data=payload, headers=headers)
    return r.json()['result']['terms']

# adds terms to project
# https://poeditor.com/docs/api#terms_add
def add_terms_json(api_token, project_id, terms_json):
    headers = {
        'Accept': 'application/json'
    }
    payload = {
        'api_token': api_token,
        'id': project_id,
        'data': json.dumps(terms_json),
    }

    r = requests.post('https://api.poeditor.com/v2/terms/add', data=payload, headers=headers)
    response_json = r.json()
    success = response_json['response']['status'] == 'success'
    result = None
    if success:
        result = json.dumps(response_json['result']['terms'], indent=2)
    return success, result

# exports and downloads translations in the given type for a given language with matching tags
# https://poeditor.com/docs/api#projects_export
# https://poeditor.com/localization/files/json
def fetch_translations(api_token, project_id, language_code, output_type = 'json', tags = None):
    headers = {
        'Accept': 'application/json'
    }
    payload = {
        'api_token': api_token,
        'id': project_id,
        'language': language_code,
        'type': output_type,
        'tags': tags,
    }

    r = requests.post('https://api.poeditor.com/v2/projects/export', data=payload, headers=headers)
    url = r.json()['result']['url']
    return requests.get(url).content


def fetch_translations_json(api_token, project_id, language_code, tags = None):
    translations = fetch_translations(api_token, project_id, language_code, 'json', tags)
    try:
        return json.loads(translations)
    except:
        return json.loads('[]')


# uploads translations in json file format for the specified language
# https://poeditor.com/docs/api#projects_upload
def upload_translations_json(api_token, project_id, language_code, translations):
    files = {'file': ('translations.json', json.dumps(translations), 'application/json')}

    headers = {
        'Accept': 'application/json'
    }
    payload = {
        'api_token': api_token,
        'id': project_id,
        'updating': 'translations',
        'language': language_code,
        'overwrite': 0,
        'fuzzy_trigger': 0,
    }

    r = requests.post('https://api.poeditor.com/v2/projects/upload', data=payload, files=files,
                      headers=headers)
    response_json = r.json()
    success = response_json['response']['status'] == 'success'
    if success:
        result = json.dumps(response_json['result']['translations'], indent=2)
    else:
        result = response_json['response']['message']
    return success, result
