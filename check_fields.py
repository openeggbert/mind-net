import os
import re

def check_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Find the struct Model definition
    # We look for 'struct Model : mindnet::model::BaseModel' or similar
    # and then capture everything until 'static constexpr auto fields'
    struct_match = re.search(r'struct\s+Model\s*:\s*[^\{]+\{([\s\S]+?)static\s+constexpr\s+auto\s+fields\s*=\s*std::make_tuple\(([\s\S]+?)\);', content)
    
    if not struct_match:
        # Try without BaseModel if it's different
        struct_match = re.search(r'struct\s+Model\s*\{([\s\S]+?)static\s+constexpr\s+auto\s+fields\s*=\s*std::make_tuple\(([\s\S]+?)\);', content)

    if struct_match:
        members_block = struct_match.group(1)
        fields_block = struct_match.group(2)

        # Extract member names
        # Members are usually 'type name;' or 'type name{...};'
        # We need to ignore methods and nested structs/enums if any
        member_names = []
        for line in members_block.split('\n'):
            line = line.strip()
            # Ignore comments, empty lines, static members, and using/methods
            if not line or line.startswith('/') or line.startswith('*') or line.startswith('static') or '(' in line or 'using' in line:
                continue
            
            # Match member variables: type name[=value]; or type name{value};
            # Types can be simple (int) or complex (std::string, enums::Type, etc.)
            # Heuristic: find the last word before ; or { or =
            m = re.match(r'^[\w:<>]+(?:\s+[\w:<>]+)*\s+(\w+)\s*(?:\{[^}]*\}|=[^;]*)?;', line)
            if m:
                name = m.group(1)
                if name not in ['public', 'protected', 'private']:
                    member_names.append(name)

        # Extract field names from tuple
        # They are usually '&Model::name,' or '&Model::name'
        field_names = re.findall(r'&Model::(\w+)', fields_block)

        if set(member_names) != set(field_names):
            print(f"File: {filepath}")
            print(f"  Members in struct: {member_names}")
            print(f"  Fields in tuple:  {field_names}")
            missing_in_tuple = set(member_names) - set(field_names)
            extra_in_tuple = set(field_names) - set(member_names)
            if missing_in_tuple:
                print(f"  MISSING in tuple: {missing_in_tuple}")
            if extra_in_tuple:
                print(f"  EXTRA in tuple:   {extra_in_tuple}")
            print("-" * 40)
            return False
    return True

files = [
    "include/mind-net-api/mindnet/plugins/core/models/AccessToken.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/ApiLog.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/AuthLog.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/Error.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/History.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/JobEntry.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/JobRun.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/LoginSession.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/RefreshToken.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/SuperAdminLog.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/Team.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/TeamMember.hpp",
    "include/mind-net-api/mindnet/plugins/core/models/User.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryFlagFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryFlag.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryIndex.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryIndexTypeFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryIndexType.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryLink.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryMap.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryNote.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryPinnedTerm.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryReview.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionarySearchFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionarySearch.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionarySource.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionarySourceTypeFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionarySourceType.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryState4.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTag.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTagTypeFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTagType.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTermAliasFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTermAlias.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTermFulltext.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTerm.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTermMetric.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTermSearch.hpp",
    "include/mind-net-plugin-dictionary/mindnet/plugins/dictionary/models/DictionaryTermVisit.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/R0State.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/R18PerfAgg.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/R18PredictionLog.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/R18State.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/R2State.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/R4State.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/RGlobalSetting.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/RReview.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/RSession.hpp",
    "include/mind-net-plugin-repetition/mindnet/plugins/repetition/models/RUserSetting.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Alert.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Annotation.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Collection.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/CollectionItem.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Content.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Flag.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Idea.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Link.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/MapCollection.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/MapCollectionItem.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Map.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Note.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/NoteNavigation.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/PinnedNote.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Project.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Property.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Question.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Source.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Tag.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/TagTypeFulltext.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/TagType.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Task.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/TermFulltext.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Term.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/TestAttemptAnswer.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/TestAttempt.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Test.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/Url.hpp",
    "include/mind-net-plugin-slip-box/mindnet/plugins/slipbox/models/WantedNote.hpp",
]

for f in files:
    check_file(f)
