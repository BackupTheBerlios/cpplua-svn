l,msg = loadlib("./test.so", "luaopen_test")
if not l then
  print(msg)
  exit()
else
  l()
end

t = create_table("ciao")
t.x = sum(5,6)
print(t.x)
