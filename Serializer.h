void writeByte(std::stringstream& ss, byte value)
{
   ss.write((const char*)(&value), sizeof(value));
}


std::string serialize(lua_State* L, const std::string source)
{
    std::stringstream ss;
    int error = luaL_loadbuffer(L, source.c_str(), source.size(), "=");
    if (error != 0)
    {
        writeByte(ss, 0x0);
        const char* error = lua_tostring(L, -1);
        std::cout << error << std::endl;
        lua_pop(L, 1);
        ss.write(error, sizeof(error));
     }
     else
     {
        const LClosure* lcl = static_cast<const LClosure*>(lua_topointer(L, -1));
        Proto* p = lcl->p;
        std::vector<const TString*> StringConstant;
        writeByte(ss, 0x1);
        std::vector<TValue*> Constant;
        Constant.clear();
       
        for (int i = 0; i < p->sizek; i++)
        {
             TValue* o = &p->k[i];
             if(o->tt == 4)
             {
                 Constant.push_back(o);
             }
        }
        writeByte(ss, Constant.size());
        for (int i = 0; i < Constant.size(); i++)
        {
             std::string kstr = getstr(&Constant[i]->tsv.len);
             writeByte(ss, kstr.length());
             for (auto& x : kstr)
             {
                 writeByte(ss, kstr.size());
             }
        }
       
        
       
     }
}
