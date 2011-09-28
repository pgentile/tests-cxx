NO_OPTIM=1

SHARED_FLAGS=-pthread

BINARIES += app
OBJS_BIN_app=app.o mutex.o thread.o logger.o

include c-project.mk
