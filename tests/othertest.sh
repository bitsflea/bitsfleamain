cleos push action bitsfleamain issue '[ "player1", "10000.0000 EOS", "memo" ]' -p bitsfleamain &&

cleos push action bitsfleamain publish '[0,{"pid":0,"uid":0,"title":"title test","description":"description 测试","photos":["photos 测试"],"category":1,"status":0,"is_new":false,"is_returns":true,"reviewer":0,"sale_method":0,"price":"0.0900 EOS","transaction_method":1,"stock_count":1,"is_retail":false,"postage":"0.0100 EOS","position":"位置","release_time":"2020-01-20T00:00:00"},null]' -p bitsfleamain -p player &&

cleos push action bitsfleamain review '[1,"reviewer",1,false,"good product"]' -p reviewer &&

cleos push action bitsfleamain placeorder '[2,"player1",1,"0"]' -p player1 &&

cleos push action bitsfleamain bindaddr '[0,"player","4,EOS","felix.x"]' -p player &&
cleos push action bitsfleamain bindaddr '[0,"player","8,NULS","necklace"]' -p player &&

cleos push action bitsfleamain issue '[ "player", "1000.0000 EOS", "memo" ]' -p bitsfleamain &&

cleos push action bitsfleamain issue '[ "fleagateways", "1000.0000 EOS", "memo" ]' -p bitsfleamain &&

cleos get table bitsfleamain bitsfleamain orders 

# pay : order id 在上一行的输出中取
# cleos transfer player1 bitsfleamain "0.1000 EOS" "p:36893488157591172661" -p player1 -c bitsfleamain
# cleos get currency balance bitsfleamain player EOS
# cleos get currency balance bitsfleamain player1 EOS
# cleos get currency balance bitsfleamain bitsfleamain EOS



# cleos push action bitsfleamain shipment '[0,"player","36893488157591172661","229988222"]' -p player
# cleos push action bitsfleamain conreceipt '[2,"player1","36893488157591172661"]' -p player1
# cleos push action bitsfleamain returns '[2,"player1","36893488157591172661","商品与描述不符"]' -p player1
# cleos push action bitsfleamain reshipment '[2,"player1","36893488157591172661","229988223"]' -p player1
# cleos push action bitsfleamain reconreceipt '[0,"player","36893488157591172661"]' -p player

# cleos transfer player bitsfleamain "0.1000 EOS" "w:0x165a3e5e0100000" -p player -c bitsfleamain
# cleos push action bitsfleamain closesettle '[0]' -p bitsfleamain
# cleos get table bitsfleamain bitsfleamain othersettle

cleos get currency balance bitsfleamain player EOS &&
cleos get currency balance bitsfleamain player1 EOS &&
cleos get currency balance bitsfleamain reviewer EOS &&
cleos get currency balance bitsfleamain fleagateways EOS &&
cleos get currency balance bitsfleamain bitsfleamain EOS

# cleos push action bitsfleamain issue '[ "player", "100.00000000 NULS", "memo" ]' -p bitsfleamain
# cleos transfer player bitsfleamain "2.0000 NULS" "w:0" -p player -c bitsfleamain