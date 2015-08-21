
# #
# DxM自带模块注册
# #
# 原型
# dxm_register_regist_module(module_name module_path)   ==> dxm_register_regist_module(module_name module_path module_name) 
# dxm_register_regist_module(module_name module_path library1 library2 library3)

# #
# dxm_register_regist_module(protobuf dxm/src/oslibs/protobuf)
# dxm_register_regist_module(libcocos2dx dxm/src/oslibs/libcocos2dx cocos2dx cocosdenshion cocos_extension box2d)

# 自定义;
dxm_register_regist_module(lib ${CMAKE_CURRENT_LIST_DIR}/templates/lib)
dxm_register_regist_module(foo ${CMAKE_CURRENT_LIST_DIR}/tests/foo)
dxm_register_regist_module(dxm_util ${CMAKE_CURRENT_LIST_DIR}/src/libs/dxm_util)
dxm_register_regist_module(dxm_com ${CMAKE_CURRENT_LIST_DIR}/src/libs/dxm_com)

# 第三方;
dxm_register_regist_module(lua ${CMAKE_CURRENT_LIST_DIR}/src/oslibs/lua)
dxm_register_regist_module(tolua ${CMAKE_CURRENT_LIST_DIR}/src/oslibs/tolua)
dxm_register_regist_module(gtest ${CMAKE_CURRENT_LIST_DIR}/src/oslibs/gtest)

# 第三方预编译;
dxm_register_regist_module(platform ${CMAKE_CURRENT_LIST_DIR}/src/prebuilt/platform pthread dl socket xml2 z inet vld)

# 预编译;
# dxm_register_regist_module(dxm_util ${CMAKE_CURRENT_LIST_DIR}/src/prebuilt/dxm_util)

# dxm_register_regist_module_finish()