#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <Runtime/Runtime.h>

#include "contracts.hpp"
#include "test_symbol.hpp"

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;

using mvo = fc::mutable_variant_object;

class platform_tester : public tester {
public:
   platform_tester() {
      create_accounts( { N(eosio.token), N(bitsfleamain), N(player), N(player1) } );
      produce_block();

      set_code( N(bitsfleamain), contracts::util::platform_wasm() );
      set_abi( N(bitsfleamain), contracts::util::platform_abi().data() );
      produce_blocks();

      const auto& acc = control->db().get<account_object,by_name>( N(bitsfleamain) );
      abi_def abi;
      BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(acc.abi, abi), true);
      abi_ser.set_abi(abi, abi_serializer_max_time);

      set_code( N(eosio.token), contracts::token_wasm() );
      set_abi( N(eosio.token), contracts::token_abi().data() );
      produce_blocks();

   }

   action_result push_action( const name& signer, const name& aname, const variant_object &data, const name& contract = N(bitsfleamain) ) {
      string action_type_name = abi_ser.get_action_type(aname);

      action act;
      act.account = contract;
      act.name    = aname;
      act.data    = abi_ser.variant_to_binary( action_type_name, data,abi_serializer_max_time );

      return base_tester::push_action( std::move(act), signer.value);
   }

   void create_currency( name manager, asset maxsupply ) {
      auto act =  mutable_variant_object()
         ("issuer",       manager )
         ("maximum_supply", maxsupply );

      base_tester::push_action(N(eosio.token), N(create), N(eosio.token), act );
   }
   void issue( name to, const asset& amount ) {
      base_tester::push_action( N(eosio.token), N(issue), N(eosio), mutable_variant_object()
                                ("to",      to )
                                ("quantity", amount )
                                ("memo", "")
                                );
   }
   void transfer( name from, name to, const string& amount, const string& memo = "" ) {
      base_tester::push_action( N(eosio.token), N(transfer), from, mutable_variant_object()
                                ("from",    from)
                                ("to",      to )
                                ("quantity", core_sym::from_string(amount) )
                                ("memo", memo )
                                );
   }

   fc::variant get_global() {
      vector<char> data = get_row_by_account( N(bitsfleamain), N(bitsfleamain), N(global), N(global) );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "global", data, abi_serializer_max_time );
   }

   void reg_new_user(const name& eosid, const string& nickname, const string& phone_hash, const string& phone_encrypt, uint64_t referrer)
   {
      base_tester::push_action( N(bitsfleamain), N(reguser), N(bitsfleamain), mutable_variant_object()
         ("eosid",eosid)
         ("nickname",nickname)
         ("phone_hash",phone_hash)
         ("phone_encrypt",phone_encrypt)
         ("referrer",referrer)
      );
   }

   fc::variant get_user(uint64_t uid) {
      vector<char> data = get_row_by_account( N(bitsfleamain), N(bitsfleamain), N(users), uid );
      BOOST_REQUIRE_EQUAL(data.empty(),false);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "User", data, abi_serializer_max_time );
   }

   fc::variant get_product( uint64_t pid ) {
      vector<char> data = get_row_by_account( N(bitsfleamain), N(bitsfleamain), N(products), pid );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "Product", data, abi_serializer_max_time );
   }

   abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(platform_tests)

BOOST_FIXTURE_TEST_CASE( recharge, platform_tester ) try {
   create_currency( N(eosio), core_sym::from_string("10000000000.0000") );
   issue( N(bitsfleamain), core_sym::from_string("1000.0000"));
   produce_blocks();

   

   base_tester::push_action( N(bitsfleamain), N(init), N(bitsfleamain), {} );

   base_tester::push_action( N(bitsfleamain), N(test), N(bitsfleamain), mutable_variant_object()("para","") );

   reg_new_user(N(player),"player","05FBED483A974456708EF12FCF3D278276E982234D775ECEE0DFA5CB49B8B492",
      "05FBED483A974456708EF12FCF3D278276E982234D775ECEE0DFA5CB49B8B492",0);

   

   produce_blocks();

   auto g = get_global();
   //BOOST_REQUIRE_EQUAL( g["total_users"].as<share_type>(), asset::from_string( g["total_point"].as_string() ).get_amount() );
   BOOST_REQUIRE_EQUAL( 1, g["total_users"] );

   auto user = get_user(0);
   BOOST_REQUIRE_EQUAL( user["nickname"].as_string(),"player");

   reg_new_user(N(player1),"player1","05FBED483A974456708EF12FCF3D278276E982234D775ECEE0DFA5CB49B8B493",
      "05FBED483A974456708EF12FCF3D278276E982234D775ECEE0DFA5CB49B8B493",0);
      
   produce_blocks();

   user = get_user(1);
   BOOST_REQUIRE_EQUAL( user["nickname"].as_string(),"player1");

   //market
   auto pro_info = mutable_variant_object()("pid",0)
                              ("title","title test")
                              ("description","description 测试")
                              ("photos","photos 测试")
                              ("category",0)
                              ("status",0)
                              ("is_new",false)
                              ("is_returns",false)
                              ("sale_method",0)
                              ("price",core_sym::from_string("100.0000"))
                              ("transaction_method",1)
                              ("postage",core_sym::from_string("1.0000"))
                              ("position","位置")
                              ("release_time","2020-01-20T00:00:00");
   auto pa_onfo = mutable_variant_object()("id",0)
                                          ("pid",0)
                                          ("security",core_sym::from_string("1.0000"))
                                          ("markup",1)
                                          ("current_price",core_sym::from_string("1.0000"))
                                          ("auction_times",0)
                                          ("last_price_user",0)
                                          ("start_time","2020-01-20T00:00:00")
                                          ("end_time","2020-01-22T00:00:00");
   auto para = mutable_variant_object()("uid", 0)
                                     ("product",pro_info)
                                     ("pa",pa_onfo);
   base_tester::push_action( N(bitsfleamain), N(publish), N(bitsfleamain), para);

   produce_blocks();

   auto pro = get_product(0);
   BOOST_REQUIRE_EQUAL( pro["title"].as_string(),"title test" );
                                
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()