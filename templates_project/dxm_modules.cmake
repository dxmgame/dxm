
# #
# DxM自带模块注册
# #
# 原型
# dxm_register_regist_module(module_name module_path)   ==> dxm_register_regist_module(module_name module_path module_name) 

# dxm_register_regist_module(module_name module_path library1 library2 library3)

# #
# dxm_register_regist_module(libcocos2dx dxm/src/oslibs/libcocos2dx cocos2dx cocosdenshion cocos_extension box2d)

# 自定义;
# dxm_register_regist_module(dxm_util ${CMAKE_CURRENT_LIST_DIR}/src/libs/dxm_util)

# 第三方;
# dxm_register_regist_module(lua ${CMAKE_CURRENT_LIST_DIR}/src/oslibs/lua)

# 第三方预编译;
# dxm_register_regist_module(platform ${CMAKE_CURRENT_LIST_DIR}/src/prebuilt/platform pthread dl socket xml2 z inet vld)

# 预编译;
# dxm_register_regist_module(dxm_util ${CMAKE_CURRENT_LIST_DIR}/src/prebuilt/dxm_util)

# dxm_register_regist_module_finish()