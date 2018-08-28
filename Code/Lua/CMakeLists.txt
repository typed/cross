
#工程名
project(libLua)

#源文件
set(SOURCE
  lapi.c
  lauxlib.c
  lbaselib.c
  lbitlib.c
  lcode.c
  lcorolib.c
  lctype.c
  ldblib.c
  ldebug.c
  ldo.c
  ldump.c
  lfunc.c
  lgc.c
  linit.c
  liolib.c
  llex.c
  lmathlib.c
  lmem.c
  loadlib.c
  lobject.c
  lopcodes.c
  loslib.c
  lparser.c
  lstate.c
  lstring.c
  lstrlib.c
  ltable.c
  ltablib.c
  ltm.c
  lundump.c
  lutf8lib.c
  lvm.c
  lzio.c
)

#unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

add_library(libLua STATIC ${SOURCE})
