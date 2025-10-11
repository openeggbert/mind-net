//
// Created by robertvokac on 10/10/25.
//

#ifndef MIND_NET_RSETTING_H
#define MIND_NET_RSETTING_H
#include <string>
#include <set>

namespace mindnet::plugins::repetition
{
    using std::string;
    using std::set;

    const string r18_b = "r18_b";
    const string r18_R_target = "r18_R_target";
    const string r18_R_opt = "r18_R_opt";
    const string r18_alpha = "r18_alpha";
    const string r18_beta = "r18_beta";
    const string r18_gamma = "r18_gamma";
    const string r18_delta = "r18_delta";
    const string r18_k_over = "r18_k_over";
    const string r18_S_min = "r18_S_min";
    const string r18_short_retry = "r18_short_retry";

    const set r_setting_keys = {
        r18_b,
        r18_R_target,
        r18_R_opt,
        r18_alpha,
        r18_beta,
        r18_gamma,
        r18_delta,
        r18_k_over,
        r18_S_min,
        r18_short_retry
    };

}
#endif //MIND_NET_RSETTING_H
