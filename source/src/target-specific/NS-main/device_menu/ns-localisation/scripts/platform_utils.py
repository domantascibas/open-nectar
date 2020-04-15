# platform-specific localization utils
import sys, yaml, json

auto_gen_string = "// THIS IS AUTO GENERATED FILE! DO NOT EDIT MANUALLY\n\n"

# remove optional region code to leave the two-letter ISO 639-1 language code
# e.g. 'zh-CN' -> 'zh', 'pt-BR' -> 'pt', 'lt' -> 'lt'
def strip_region(code):
    return code.split('-', 1)[0]

def term(obj):
    return obj["term"]

def definition(obj):
    return obj["term"] if obj["definition"] == None else obj["definition"]

def map_mobile_data(data, lang_code):
    array  = json.loads(data)
    result = auto_gen_string
    result += "export default {\n"
    for obj in array:
        result += "  " + term(obj) + ": \"" + definition(obj) + "\",\n"
    result += "}\n"
    return result

def map_mbed_data(data, lang_code):
    array  = json.loads(data)
    result = auto_gen_string
    result += "#include \"keys.h\"\n"
    result += "#include <string>\n\n"
    result += "static const std::string " + lang_code + "_string(const LocalizationKey &key) {\n"
    result += "  switch (key) {\n"
    for obj in array:
        result += "  case " + term(obj) + ":\n"
        result += "    return \"" + definition(obj) + "\";\n"
    result += "  }\n"
    result += "};\n"
    
    return result

def map_web_data(data, lang_code):
    array  = json.loads(data)
    result = yaml.safe_dump(array, default_flow_style=False)
    return result