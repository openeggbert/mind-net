///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : Mind map software.
// Copyright (C) 2025-2025 the original author or authors.
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <https://www.gnu.org/licenses/> or write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
///////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @author <a href="mailto:robertvokac@robertvokac.com">Robert Vokac</a>
 */

#include <string>

#include "mindnet/enums/Crudl.h"
#include "mindnet/persistence/api/IRepository.h"
#include "mindnet/persistence/impl/sqlite/RepositoryHelper.h"
#include "mindnet/models/columns/UserColumns.h"
#include "mindnet/models/columns/MessageColumns.h"
#include "mindnet/models/columns/TeamColumns.h"
#include "mindnet/models/columns/TeamMemberColumns.h"
#include "mindnet/models/columns/DiscussionColumns.h"
#include "mindnet/models/columns/CommentColumns.h"
#include "mindnet/models/columns/SuggestionColumns.h"
#include "mindnet/models/columns/SuggestionReviewColumns.h"
#include "mindnet/models/columns/HistoryColumns.h"
#include "mindnet/models/columns/MapColumns.h"
#include "mindnet/models/columns/ContentColumns.h"
#include "mindnet/models/columns/NoteColumns.h"
#include "mindnet/models/columns/PropertyColumns.h"
#include "mindnet/models/columns/TagTypeColumns.h"
#include "mindnet/models/columns/TagColumns.h"
#include "mindnet/models/columns/CollectionColumns.h"
#include "mindnet/models/columns/CollectionItemColumns.h"
#include "mindnet/models/columns/ReviewColumns.h"
#include "mindnet/models/columns/SM2StateColumns.h"
#include "mindnet/models/columns/ReferenceColumns.h"
#include "mindnet/models/columns/LinkColumns.h"
#include "mindnet/persistence/impl/sqlite/repositories/Convertors.h"

namespace mindnet::persistence::impl::sqlite::repositories
{
    entity_fields request_to_entity_fields_user(
        crow::json::rvalue& body, enums::Crudl crudl)
    {
        start_of_request_to_entity_fields(User)

        mandatory_string(USERNAME);
        mandatory_string(PASSWORD_HASH);
        optional_string(DISPLAY_NAME, "");
        mandatory_int(ROLE);
        optional_string(PROFILE_TEXT, "");
        optional_int(LAST_LOGIN, 0);
        optional_string(EMAIL, "");
        mandatory_int(STATUS);
        return fields;
    }

    entity_fields request_to_entity_fields_message(method_arguments())
    {
        start_of_request_to_entity_fields(Message)

        int_foreign_key(OWNER_ID)
        int_foreign_key(SENDER_ID)
        int_foreign_key(RECIPIENT_ID)
        optional_string(SUBJECT, "")
        optional_int(IMPORTANT, 0)
        mandatory_string(BODY)
        optional_int(SENT_AT, 0)
        optional_int(SYSTEM_MESSAGE, 0)
        optional_string(FOLDER, "")
        optional_int(DRAFT, 0)
        optional_int(IS_READ, 0)
        optional_int(DELETED_AT, 0)
        optional_int(STARRED, 0)

        return fields;
    }

    entity_fields request_to_entity_fields_team(method_arguments())
    {
        start_of_request_to_entity_fields(Team)
        mandatory_string(NAME)
        optional_string(DESCRIPTION, "")
        int_foreign_key(CREATED_BY)
        int_foreign_key(LEADER_ID)

        return fields;
    }

    entity_fields request_to_entity_fields_team_member(method_arguments())
    {
        start_of_request_to_entity_fields(TeamMember)
        int_foreign_key(TEAM_ID)
        int_foreign_key(USER_ID)
        mandatory_int(ROLE)
        mandatory_int(JOINED_AT)
        optional_int(IS_ACTIVE, 1)
        optional_int(LEFT_AT, 0)

        return fields;
    }

    entity_fields request_to_entity_fields_discussion(method_arguments())
    {
        start_of_request_to_entity_fields(Discussion)
        int_foreign_key(TEAM_ID)
        mandatory_string(TITLE)
        int_foreign_key(CREATED_BY)
        optional_int(IS_PINNED, 0)
        return fields;
    }

    entity_fields request_to_entity_fields_comment(method_arguments())
    {
        start_of_request_to_entity_fields(Comment)
        int_foreign_key(DISCUSSION_ID)
        int_foreign_key(USER_ID)
        mandatory_string(CONTENT)
        int_foreign_key(PARENT_COMMENT_ID)
        optional_int(IS_DELETED, 0)
        return fields;
    }

    entity_fields request_to_entity_fields_suggestion(method_arguments())
    {
        start_of_request_to_entity_fields(Suggestion)
        return fields;
    }

    entity_fields request_to_entity_fields_suggestion_review(method_arguments())
    {
        start_of_request_to_entity_fields(SuggestionReview)
        return fields;
    }

    entity_fields request_to_entity_fields_history(method_arguments())
    {
        start_of_request_to_entity_fields(History)

        mandatory_string(TABLE_NAME);
        mandatory_int(RECORD_ID);
        mandatory_int(OPERATION);
        mandatory_string(DATA_JSON);
        optional_string(REASON, "");
        return fields;
    }

    entity_fields request_to_entity_fields_map(method_arguments())
    {
        start_of_request_to_entity_fields(Map)

        mandatory_string(NAME);
        optional_string(DESCRIPTION, "");
        optional_string(CATEGORY, "");
        int_foreign_key(OWNER_ID);
        int_foreign_key(TEAM_ID);
        mandatory_int(OWNER_RIGHTS);
        mandatory_int(TEAM_RIGHTS);
        mandatory_int(OTHER_RIGHTS);
        return fields;
    }

    entity_fields request_to_entity_fields_content(method_arguments())
    {
        start_of_request_to_entity_fields(Content)

        mandatory_string(VALUE);
        mandatory_int(FORMAT);
        mandatory_int(VERSION);

        return fields;
    }

    entity_fields request_to_entity_fields_note(method_arguments())
    {
        start_of_request_to_entity_fields(Note)

        mandatory_int(MAP_ID);
        mandatory_int(SIBLING_POSITION);
        mandatory_string(TITLE);

        int_foreign_key(CONTENT_ID)
        int_foreign_key(PARENT_NOTE_ID)

        optional_int(IMPORTANCE, 0);
        optional_int(DIFFICULTY, 0);
        return fields;
    }

    entity_fields request_to_entity_fields_property(method_arguments())
    {
        start_of_request_to_entity_fields(Property)

        mandatory_int(MAP_ID);
        mandatory_int(NOTE_ID);
        mandatory_string(KEY);
        optional_string(VALUE, "");
        return fields;
    }

    entity_fields request_to_entity_fields_tag_type(method_arguments())
    {
        start_of_request_to_entity_fields(TagType)

        mandatory_int(MAP_ID);
        mandatory_string(TITLE);
        return fields;
    }

    entity_fields request_to_entity_fields_tag(method_arguments())
    {
        start_of_request_to_entity_fields(Tag)

        mandatory_int(NOTE_ID);
        mandatory_int(TAG_TYPE_ID);
        return fields;
    }

    entity_fields request_to_entity_fields_collection(method_arguments())
    {
        start_of_request_to_entity_fields(Collection)

        mandatory_string(NAME);
        optional_string(DESCRIPTION, "");
        optional_int(ORDER_INDEX, 0);
        int_foreign_key(CREATED_BY);
        optional_int(IS_PUBLIC, 0);
        return fields;
    }

    entity_fields request_to_entity_fields_collection_item(method_arguments())
    {
        start_of_request_to_entity_fields(CollectionItem)

        mandatory_int(COLLECTION_ID);
        mandatory_int(NOTE_ID);
        optional_int(ORDER_INDEX, 0);
        return fields;
    }

    entity_fields request_to_entity_fields_review(method_arguments())
    {
        start_of_request_to_entity_fields(Review)

        mandatory_int(USER_ID);
        mandatory_int(NOTE_ID);
        mandatory_int(REVIEW_DATE);
        mandatory_int(GRADE);
        optional_string(RESPONSE_DATA, "");
        optional_string(NOTES, "");
        return fields;
    }

    entity_fields request_to_entity_fields_sm2_state(method_arguments())
    {
        start_of_request_to_entity_fields(SM2State)

        mandatory_int(USER_ID);
        mandatory_int(NOTE_ID);
        mandatory_int(REPETITIONS);
        mandatory_int(INTERVAL);
        mandatory_int(EF_TIMES_100);
        mandatory_int(NEXT_REVIEW);
        mandatory_int(LAST_REVIEW);
        mandatory_int(LAST_QUALITY);
        return fields;
    }

    entity_fields request_to_entity_fields_reference(method_arguments())
    {
        start_of_request_to_entity_fields(Reference)

        mandatory_int(FROM_NOTE_ID);
        mandatory_int(TO_NOTE_ID);
        optional_string(LABEL, "");
        return fields;
    }

    entity_fields request_to_entity_fields_link(method_arguments())
    {
        start_of_request_to_entity_fields(Link)

        mandatory_int(FROM_NOTE_ID);
        mandatory_string(TO_URL);
        return fields;
    }
}
