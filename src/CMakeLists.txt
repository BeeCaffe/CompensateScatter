PROJECT(CompensateScatter)
SET(
        SRC
        scatter.cpp
        utils.cpp
)
ADD_LIBRARY(CompensateScatter SHARED ${SRC} ../main/main.cpp)
ADD_LIBRARY(CompensateScatter_STATIC STATIC ${SRC})
SET_TARGET_PROPERTIES(CompensateScatter_STATIC PROPERTIES OUTPUT_NAME "CompensateScatter")
SET_TARGET_PROPERTIES(CompensateScatter PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(CompensateScatter_STATIC PROPERTIES CLEAN_DIRECT_OUTPUT 1)