//
// Created by robertvokac on 9/6/25.
//
#ifndef SUGGESTIONPERSISTENCEMETHODS_H
#define SUGGESTIONPERSISTENCEMETHODS_H

#include <utility>

#include "mindnet/api/PersistenceMethods.h"
#include "mindnet/api/RequestContext.h"
#include "mindnet/plugins/core/enums/SingleRight.h"
#include "mindnet/api/AccessTokenContext.h"
#include "mindnet/plugins/suggestion/models/Suggestion.h"
#include "mindnet/plugins/suggestion/models/SuggestionReview.h"

namespace mindnet::plugins::slipbox::models
{
    struct User;
}

namespace mindnet::plugins::slipbox
{

    gen_find_h(suggestion, Suggestion, suggestion)
    gen_find_h(suggestion, SuggestionReview, suggestion_review)

}


#endif // SUGGESTIONPERSISTENCEMETHODS_H
