QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_CXXFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_LFLAGS += -O3

SOURCES += \
        cprogram.cc \
        dataset.cpp \
        doublestack.cc \
        fparser.cc \
        get_options.cc \
        main.cpp \
        population.cc \
        problem.cc \
        program.cc \
        rule.cc \
        ruleprogram.cpp \
        simanmethod.cpp \
        symbol.cc \
        tolmin.cc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cprogram.h \
    dataset.h \
    doublestack.h \
    fparser.hh \
    get_options.h \
    population.h \
    problem.h \
    program.h \
    rule.h \
    ruleprogram.h \
    simanmethod.h \
    symbol.h
