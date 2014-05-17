TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += libs
message("start")

DEF = false
CONFIG(nodeprog) {
     message("nodeprog")
    SUBDIRS += nodeprog
    DEF = true
}

CONFIG(demogui) {
    message("demogui")
    SUBDIRS += demogui
    DEF = true
}

CONFIG(relayprog) {
    message("relayprog")
    SUBDIRS += relayprog
    DEF = true
}


equals(DEF,false) {
    message("else")
    SUBDIRS += demogui \
                nodeprog \
                relayprog
}
