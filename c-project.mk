# Build a C / C++ project

CC=cc
CPPC=g++
LD=$(CPPC)
AR=ar

ifndef SRC_DIR
	SRC_DIR=src
endif

ifndef INCLUDE_DIR
	INCLUDE_DIR=./include
endif

ifndef BUILD_DIR
	BUILD_DIR=./build
endif

ifndef BIN_DIR
	BIN_DIR=./bin
endif

ifndef LIB_DIR
	LIB_DIR=./lib
endif

CC_FLAGS += -ansi -pedantic -Wall -Werror

ifdef SHARED_DEFINES
	CC_FLAGS += $(addprefix -D ,$(SHARED_DEFINES))
endif

CC_FLAGS += -I$(INCLUDE_DIR)

ifdef SHARED_INCLUDE_DIRS
	CC_FLAGS += $(addprefix -I,$(SHARED_INCLUDE_DIRS))
endif

LD_FLAGS += $(if $(or $(LIBS),$(STATIC_LIBS)),-L$(LIB_DIR))

ifdef SHARED_LIB_DIRS
	LD_FLAGS += $(addprefix -L,$(SHARED_LIB_DIRS))
endif

ifdef SHARED_LIB_NAMES
	LD_FLAGS += $(addprefix -l,$(SHARED_LIB_NAMES))
endif

ifdef SHARED_FLAGS
	CC_FLAGS += $(SHARED_FLAGS)
	LD_FLAGS += $(SHARED_FLAGS)
endif

ifndef OPTION_NO_OPTIM
	ifndef NO_OPTIM
		CC_FLAGS += -O2
		LD_FLAGS += -O2
	endif
endif

# Create binary name
#   $(1): binary name
define gen_binary_name
	$(addprefix $(BIN_DIR)/,$(1))
endef

# Create library name
#   $(1): library name
define gen_lib_name
	$(addprefix $(LIB_DIR)/lib,$(addsuffix .so,$(1)))
endef

# Create static library name
#   $(1): library name
define gen_static_lib_name
	$(addprefix $(LIB_DIR)/lib,$(addsuffix .a,$(1)))
endef

# Binary build template
#   $(1): binary name
define build_binary_tpl
$$(call gen_binary_name,$(1)): \
		$$(addprefix $(BUILD_DIR)/,$$(value OBJS_BIN_$(1))) \
		| $(BIN_DIR)
	$(LD) $(LD_FLAGS) \
			$$(addprefix -L,$$(value LIB_DIRS_BIN_$(1))) \
			$$(addprefix -l,$$(value LIB_NAMES_BIN_$(1))) \
			-o $$@ $$(filter %.o,$$^)
endef

# Shared object build template
#   $(1): shared object name
define build_lib_tpl
$$(call gen_lib_name,$(1)): \
		$$(addprefix $(BUILD_DIR)/,$$(value OBJS_LIB_$(1))) \
		| $(LIB_DIR)
	$(LD) $(LD_FLAGS) \
			$$(addprefix -L,$$(value LIB_DIRS_LIB_$(1))) \
			$$(addprefix -l,$$(value LIB_NAMES_LIB_$(1))) \
			-shared -o $$@ $$(filter %.o,$$^)
endef

# Shared object build template
#   $(1): shared object name
define build_static_lib_tpl
$$(call gen_static_lib_name,$(1)): \
		$$(addprefix $(BUILD_DIR)/,$$(value OBJS_STATIC_LIB_$(1))) \
		| $(LIB_DIR)
	$(AR) rcs $$@ $$(filter %.o,$$^)
endef

# Build all binaries and librairies
all: $(call gen_binary_name,$(BINARIES))
all: $(call gen_lib_name,$(LIBS))
all: $(call gen_static_lib_name,$(STATIC_LIBS))

# Clean all
clean:
	rm -fr $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)

# Build C sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -c -MMD -o $@ $<

# Build C++ sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CPPC) $(CC_FLAGS) -c -MMD -o $@ $<

# Build required directories
$(BUILD_DIR) $(BIN_DIR) $(LIB_DIR):
	mkdir $@

# Include all generated dependencies files
include $(wildcard $(BUILD_DIR)/*.d)

# Generating build rules
$(foreach binary,$(BINARIES),$(eval $(call build_binary_tpl,$(binary))))
$(foreach lib,$(LIBS),$(eval $(call build_lib_tpl,$(lib))))
$(foreach lib,$(STATIC_LIBS),$(eval $(call build_static_lib_tpl,$(lib))))

# Default goal
.DEFAULT_GOAL := all
