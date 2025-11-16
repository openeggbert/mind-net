//
// Created by robertvokac on 10/10/25.
//

#pragma once

#include <string>
#include <set>

namespace mindnet::plugins::repetition
{
    using std::string;
    using std::set;

    const string b = "b";
    const string R_target = "R_target";
    const string R_opt = "R_opt";
    const string alpha = "alpha";
    const string beta = "beta";
    const string gamma = "gamma";
    const string delta = "delta";
    const string k_over = "k_over";
    const string S_min = "S_min";
    const string short_retry = "short_retry";

    const set r_setting_keys = {
        b,
        R_target,
        R_opt,
        alpha,
        beta,
        gamma,
        delta,
        k_over,
        S_min,
        short_retry
    };
}
