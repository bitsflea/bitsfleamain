#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include <string>

#include "global.hpp"
#include "types.hpp"

#include "token/token_stats.hpp"
#include "token/token_balance.hpp"

#include "user.hpp"
#include "product.hpp"
#include "review.hpp"
#include "coins.hpp"
#include "tablelog.hpp"

using namespace eosio;

namespace rareteam {

    CONTRACT bitsfleamain : public contract {
    private:
        /**
         * Whether it has been initialized
         */ 
        bool                    _is_init = false;
        flea_global_singleton   _global_table;
        global                  _global;
        user_index              _user_table;
        tablelog_index          _tablelog_table;

        global get_default_global() {
            global g;
            return g;
        }

        /*****token*****/
        void sub_balance( name owner, asset value );
        void add_balance( name owner, asset value, name ram_payer );
        /*****token end*****/

        void BidAuction( uint64_t buyer_uid, const name& buyer_eosid, uint32_t pid, const asset& price );
        void PayOrder( uint128_t order_id, const asset& quantity );
        void EndOrder( const Order& order );
        void Refund( const Order& order );
        bool CheckReviewer( uint64_t reviewer_uid, bool is_new = false );
        void SubCredit( const User& user, uint32_t value, bool cancel_reviewer = false );
        void SubCredit( uint64_t user_uid, uint32_t value, bool cancel_reviewer = false );
        void AddCredit( const User& user, uint32_t value );
        void AddCredit( uint64_t user_uid, uint32_t value );
        void RewardReferrer( const name& referrer );
        void Settle( const Order& order, const User& seller, const User& buyer);
        bool IsLockUser( uint64_t user_uid );
        bool IsLockUser( const User& user );
        bool CheckSymbol( const symbol& symbol );
        void PayCoin( const string& stroid, const Order& order, const User& seller, const User& buyer, const asset& seller_income, const asset& devops_income, const asset& referrer_income, const name& contract);
        void Withdraw( const name& user_eosid, const asset& quantity );

        void AddTableLog(const name& table, OpType type, uint32_t primary);
        void AddTableLog(const name& table, OpType type, uint64_t primary);
        void AddTableLog(const name& table, OpType type, uint128_t primary);
        void DeleteTableLog(uint64_t id);

    public:
        bitsfleamain( name receiver, name code, datastream<const char*> ds );
        ~bitsfleamain() {
            if( _is_init ){
                _global_table.set( _global, _self );
            }
        }

        /*****fleamain management*****/
        ACTION init();
        // ACTION reset();
        ACTION update(const string& para);
        ACTION cleanscope( const name& n);
        ACTION setcfg( const uint32_t index, const double value );

        void OnError( const onerror& error );
        /*****fleamain management end*****/

        /*****Token management*****/
        ACTION create( name issuer, asset maximum_supply, int16_t team );
        ACTION issue( name to, asset quantity, string memo );
        ACTION retire( asset quantity, string memo );
        ACTION transfer( const name& from, const name& to, const asset& quantity, const string& memo );
        ACTION claim( name to, asset quantity );
        ACTION closetoken( name owner, const asset& symbol );
        /*****Token management end*****/

        /********platform*********/
        /**
         * 注册新用户到平台(需要平台签名)
         */ 
        ACTION reguser( const name& eosid, const string& nickname, const checksum256& phone_hash, const string& phone_encrypt, uint64_t referrer, const public_key& auth_key );

        /**
         * 用户设置个人信息(需要用户签名)
         */ 
        ACTION setprofile(const name& eosid, const optional<string>& nickname, const optional<string>& head);
        /**
         * 申请成为评审员(需要用户签名)
         */ 
        ACTION appreviewer( uint64_t uid, const name& eosid );
        /**
         * 用户为评审员投票(一人可以投多个评审员)(需要用户签名)
         */ 
        ACTION votereviewer( uint64_t voter_uid, const name& voter_eosid, uint64_t reviewer_uid, bool is_support );
        /**
         * 发布商品(需要平台签名)
         */ 
        ACTION publish( uint64_t uid, const Product& product, const optional<ProductAuction>& pa );
        /**
         * 评审员审核商品(需要用户签名)
         */ 
        ACTION review( uint64_t reviewer_uid, const name& reviewer_eosid, uint32_t pid, bool is_delisted, optional<string>& memo );
        /**
         * 卖家下架商品(需要用户签名)
         */ 
        ACTION pulloff( uint64_t seller_uid, const name& seller_eosid, uint32_t pid );
        /**
         * 下单(需要用户、平台签名)
         */ 
        ACTION placeorder( uint64_t buyer_uid, const name& buyer_eosid, uint32_t pid, const uint128_t& order_id, uint32_t to_addr, optional<string>& pay_addr);
        /**
         * 取消订单
         */ 
        ACTION cancelorder( uint64_t buyer_uid, const name& buyer_eosid, const uint128_t& order_id);
        /**
         * 清除过期订单
         */ 
        ACTION cleanorder( uint64_t start_id );
        /**
         * 发货(需要用户签名)
         */ 
        ACTION shipment( uint64_t seller_uid, const name& seller_eosid, uint128_t order_id, const string& number );
        /**
         * 退货时的发货(需要用户签名)
         */ 
        ACTION reshipment( uint64_t buyer_uid, const name& buyer_eosid, uint128_t order_id, const string& number );
        /**
         * 确认收货(需要用户签名)
         */ 
        ACTION conreceipt( uint64_t buyer_uid, const name& buyer_eosid, uint128_t order_id );
        /**
         * 退货时的确认收货(需要用户签名)
         */ 
        ACTION reconreceipt( uint64_t seller_uid, const name& seller_eosid, uint128_t order_id );
        /**
         * 买家发起退货(需要用户签名)
         */ 
        ACTION returns( uint64_t buyer_uid, const name& buyer_eosid, uint128_t order_id, const string& reasons, uint32_t to_addr );
        /**
         * 申请仲裁(需要用户签名)
         */ 
        ACTION applyarbit( uint64_t plaintiff_uid, const name& plaintiff_eosid, const Arbitration& arbitration );
        /**
         * 评审员参与仲裁(需要用户签名)
         */ 
        ACTION inarbit( uint64_t reviewer_uid, const name& reviewer_eosid, uint32_t arbit_id );
        /**
         * 更新仲裁结果(需要用户签名)
         */ 
        ACTION updatearbit( const Arbitration& arbit );
        /**
         * 延迟收货(需要用户签名)
         */ 
        ACTION deferreceipt( uint64_t user_uid, const name& user_eosid, uint128_t order_id );
        /**
         * 退货时的延迟收货(需要用户签名)
         */ 
        ACTION deferreturn( uint64_t user_uid, const name& user_eosid, uint128_t order_id );
        /**
         * 开始链外同步用户数据(需要平台签名)
         */ 
        ACTION startsync();
        /**
         * 结束链外同步数据(需要平台签名)
         */ 
        ACTION endsync();
        /**
         * 完成提现(需要平台签名)
         */ 
        ACTION closesettle( uint64_t os_id, const string& trx_id );
        /**
         * 绑定外链账号或者地址(需要用户签名)
         */ 
        ACTION bindaddr( uint64_t uid, const name& user_eosid, const symbol& sym, const string& addr );
        /**
         * 添加、更新支持币种(需要平台签名)
         */ 
        ACTION updatecoin( const symbol& sym, const asset& fee, bool is_out, const asset& max );
        /**
         * 设置订单的支付地址，只有订单支付币种不支持memo时才用(需要平台签名)
         */ 
        ACTION setpayaddr( const uint128_t& order_id, const string& addr );

        /**
         * 删除同步日志
         */ 
        ACTION deletelog( uint64_t id);
        
        
        /********platform End*****/
        /**
         * 接收下单
         * 下单memo格式如: p:36893488153296537970
         */ 
        // notify
        [[eosio::on_notify("eosio.token::transfer")]]
        void OnEOSTransfer( const name& from, const name& to, const asset& quantity, const string& memo );
        /**
         * 接收下单或提现
         * 下单memo格式如: p:36893488153296537970
         * 提现memo格式如: w:player1
         */ 
        void OnMyTransfer( const name& from, const name& to, const asset& quantity, const string& memo );

        /**
         * 跨链支付
         * 
         */ 
        [[eosio::on_notify("bosibc.io::transfer")]]
        void OnIBCTransfer( const name& from, const name& to, const asset& quantity, const string& memo );


    public:
        // static const user& GetUser( const name& tokenContractAccount, const name& account )
        // {
        //     user_index users( tokenContractAccount, tokenContractAccount.value );
        //     const auto& acc = users.get( account.value, "unable to find account" );
        //     return acc;
        // }

        // static const user FindUser( const name& tokenContractAccount, const name& account )
        // {
        //     user_index users( tokenContractAccount, tokenContractAccount.value );
        //     user u;
        //     auto aitr = users.find( account.value );
        //     if( aitr != users.end() ) {
        //         u = *aitr;
        //     }
        //     return u;
        // }
    
        // static void CheckUser( const name& tokenContractAccount, const name& account )
        // {
        //     user_index users( tokenContractAccount, tokenContractAccount.value );
        //     auto itr = users.find( account.value );
        //     eosio_assert( itr != users.end(), "unknown account" );
        //     eosio_assert( itr->status == 0, "this account has been locked" );
        //     eosio_assert( itr->game_id.value == 0, "already in the game" );
        // }


        /*****token*****/
        static asset GetSupply( name tokenContractAccount, symbol_code symCode )
        {
            tokenStats statstable( tokenContractAccount, symCode.raw() );
            const auto& st = statstable.get( symCode.raw() );
            return st.supply;
        }

        static asset GetBalance( name tokenContractAccount, name owner, symbol_code symCode )
        {
            balances accountstable( tokenContractAccount, owner.value );
            const auto& ac = accountstable.get( symCode.raw() );
            return ac.balance;
        }
        /*****token end*****/

    };
}