#pragma once

#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <string>

#include "types.hpp"

using namespace std;

namespace rareteam {

    struct [[eosio::table, eosio::contract("bitsfleamain")]] global {
        /**
         * The total amount of users in the platform.
         */
        uint32_t total_users = 0;

        /**
         * System fee ratio
         */ 
        double fee_ratio = 0.01;

        /************************Points related************************/
        /**
         * the referral commission
         */ 
        double ref_commission_scale = 0.01;
        /**
         * Points gift once by the system at the time of referral
         */ 
        asset ref_sys_gift = asset( 50000, FMP );
        /**
         * Referral gift pool
         */ 
        asset ref_pool = asset( 0, FMP );
        /**
         * transaction gift pool
         */ 
        asset transaction_pool = asset( 0, FMP );
        /**
         * salary gift pool
         */ 
        asset salary_pool = asset( 0, FMP );
        /************************Points related End************************/

        /*******************credit related***************/
        
        /**
         * Basic credit score
         */ 
        int32_t credit_base_score = 500;
        /**
         * Credit limit for participation in referrals
         */ 
        int32_t credit_ref_limit = 500;

        /**
         * Lower credit limit for reviewers
         */
        int32_t credit_reviewer_limit = 500;
        /*******************credit related End***********/

        /*******************reviewer related**************************/
        /**
         * Minimum number of reviewers
         */ 
        uint32_t review_min_count = 3;
        /**
         * Maximum number of reviewers
         */ 
        uint32_t review_max_count = 3000;
        /**
         * Reviewer daily base salary (Not used)
         */ 
        asset review_salary_base = asset( 100000, FMP );
        /**
         * Review the salary of a product
         */ 
        asset review_salary_product = asset( 100000, FMP );
        /**
         * Reviewer's salary for each dispute
         */ 
        asset review_salary_dispute = asset( 200000, FMP );
        /*******************reviewer related End**************************/
        /**
         * Project profit list
         */ 
        vector<asset> income;
        /**
         * the project start time.
         */ 
        time_point_sec project_start_time = time_point_sec(0);
        /**
         * Team FMP share unlock days
         */ 
        uint32_t team_unlock_time = 365;
        /**
         * pay time out 8 hours
         */ 
        uint32_t pay_time_out = 8 * 60 * 60;
        /**
         * ship time out 24 hours
         */ 
        uint32_t ship_time_out = 24 * 60 * 60;
        /**
         * receipt time out 7 days
         */ 
        uint32_t receipt_time_out = 7 * 24 * 60 * 60;
        /**
         * Deferred up to 3 times
         */ 
        uint32_t max_deferr_times = 3;
    };

   typedef eosio::singleton< "global"_n, global> flea_global_singleton;
}