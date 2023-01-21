local json = require ("dkjson")

local str = [[
{
  "numbers": [ 2, 3, -20.23e+2, -4 ],
  "currency": "\u20AC"
}
]]

local obj, pos, err = json.decode (str, 1, nil)
if err then
  print ("Error:", err)
else
  print(obj.numbers[1])
  print ("currency", obj.currency)
  for i = 1,#obj.numbers do
    print (i, obj.numbers[i])
  end
end
