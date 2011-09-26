ZMQ_BASE_DIR=$(HOME)/Programmation/DevTools/zeromq

NO_OPTIM=1

SHARED_FLAGS=-pthread

#SHARED_INCLUDE_DIRS=$(ZMQ_BASE_DIR)/include
#SHARED_LIB_DIRS=$(ZMQ_BASE_DIR)/lib
#SHARED_LIB_NAMES=zmq pthread

BINARIES += ticketconsumer
OBJS_BIN_ticketconsumer=ticketconsumer.o mutex.o thread.o logger.o

LIBS += logger
OBJS_LIB_logger=logger.o mutex.o thread.o

include c-project.mk
