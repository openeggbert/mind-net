//
// Created by robertvokac on 9/6/25.
//

#include "mindnet/plugins/suggestion/SuggestionPersistenceMethods.h"

#include "mindnet/api/IService.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/api/ValidatorBase.h"
#include "mindnet/plugins/suggestion/models/Suggestion.h"
#include "mindnet/plugins/suggestion/models/SuggestionReview.h"

namespace mindnet::plugins::slipbox
{
    gen_find_cpp(suggestion, Suggestion, suggestion, SUGGESTION)
    gen_find_cpp(suggestion, SuggestionReview, suggestion_review, SUGGESTION_REVIEW)
}
