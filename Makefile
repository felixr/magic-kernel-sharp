default: d/all

INCLUDES = true

CC = clang
MKDIR_P = mkdir -p
CP = cp
CP_R = $(CP) -R
RM = rm
RM_F = $(RM) -f
RM_RF = $(RM) -rf
TAR_CZF = tar -czf
DSYMUTIL = dsymutil
UNAME_S = $(shell uname -s)
FIND = find
TOUCH = -exec touch {} +

CPPFLAGS = -Ilib -I/usr/local/include

DBG = dbg
OPT = opt

MODES = $(DBG) $(OPT)

CFLAGS = -ansi -pedantic -Wall -Werror
CFLAGS/$(DBG) = -O1 -g -DJPC_DEBUG -fsanitize=address -fno-omit-frame-pointer
CFLAGS/$(OPT) = -O3

LDFLAGS = -Wall -L/usr/local/lib -lm -lpthread
LDFLAGS/$(DBG) = -fsanitize=address
LDFLAGS/$(OPT) =

ifeq ($(UNAME_S),Darwin)
  POST_LD/$(DBG) = && $(DSYMUTIL) $$$$@
  ECHO_E = echo
else
  POST_LD/$(DBG) =
  ECHO_E = echo -e
endif
POST_LD/$(OPT) =

BUILD = build

LIB = lib
PROG = prog
TEST = test

GZ_FILE = $(LIB)/ExternalFile.h
PNG_FILE = $(LIB)/png_file.h
JPEG_FILE = $(LIB)/jpeg_file.h

LSAN = ASAN_OPTIONS=detect_leaks=

SRC_DIRS = $(LIB) $(PROG) $(TEST)

LIBS = $(basename $(notdir $(wildcard $(LIB)/*.c)))
PROGS = $(basename $(notdir $(wildcard $(PROG)/*.c)))
TESTS = $(basename $(notdir $(wildcard $(TEST)/*.c)))

BIN = bin
BMK = bmk
OBJ = obj
OMK = omk

BUILD_DIRS = $(BIN) $(BMK) $(OBJ) $(OMK)
BUILD_PATHS = $(foreach m,$(MODES),\
  $(foreach d,$(BUILD_DIRS),$(BUILD)/$(m)/$(d)/))

TARGETS = $(PROGS) $(TESTS)
OBJECTS = $(LIBS) $(TARGETS)

REGULAR_TESTS = $(filter-out %_death %Death,$(TESTS))
DEATH_TESTS = $(filter %_death %Death,$(TESTS))

DIST = dist
REPO = repo

DMK = dmk
RMK = rmk

SRC = src
TARGZ = targz

DIST_PATHS = $(DIST)/ $(DIST)/$(DMK)/ $(DIST)/$(SRC)/ $(DIST)/$(TARGZ)/
$(foreach e,$(PROGS) $(TESTS),$(eval DIST_PATHS += $(DIST)/$(SRC)/$(e)/$(LIB)/ \
  $(DIST)/$(SRC)/$(e)/$(PROG)/ $(DIST)/$(SRC)/$(e)/$(TEST)/))

DIST_DOCS = Makefile _LICENSE _README

REPOS_MK = $(REPO)/repos.mk

ifdef INCLUDES
-include $(REPOS_MK)
endif

REPO_PATHS = $(REPO)/ $(REPO)/$(RMK)/ $(REPO)/$(SRC)/ $(REPO)/$(TARGZ)/
$(foreach r,$(REPOS),$(eval REPO_PATHS += $(REPO)/$(SRC)/$(r)/$(LIB)/ \
  $(REPO)/$(SRC)/$(r)/$(PROG)/ $(REPO)/$(SRC)/$(r)/$(TEST)/))

CREATE_PATHS = $(BUILD_PATHS) $(DIST_PATHS) $(REPO_PATHS)

CHECKS = \
  UNAME_S \
  SRC_DIRS \
  LIBS \
  PROGS \
  TESTS \
  BUILD_PATHS \
  DIST_PATHS \
  CREATE_PATHS \
  TARGETS \
  OBJECTS \
  DTARGETS \
  OTARGETS \
  ATARGETS \
  REGULAR_TESTS \
  DEATH_TESTS \
  EXECUTE_TESTS_DBG \
  EXECUTE_TESTS_OPT \
  CLEAN_MKS \
  REPOS \
  CREATE_PATHS \

check:
	@$(foreach v, $(CHECKS), echo; echo $(v) = $($(v));)

clean:
	$(RM_RF) $(BUILD) $(DIST) $(REPO)/*/

clean/d:
	$(RM_RF) $(BUILD)/$(DBG)

clean/o:
	$(RM_RF) $(BUILD)/$(OPT)

clean/dist:
	$(RM_RF) $(DIST)

clean/repo:
	$(RM_RF) $(REPO)/*/

define cleaners
clean/$(1)/$(2):
	$(RM_RF) $(BUILD)/$(3)/$(2)
endef
$(foreach d,$(BUILD_DIRS),$(eval $(call cleaners,d,$(d),$(DBG))))
$(foreach d,$(BUILD_DIRS),$(eval $(call cleaners,o,$(d),$(OPT))))

define clean_mks
clean/$(1)/$(2):
	$(RM) $(foreach m,$(MODES),$(BUILD)/$(m)/$(1)/$(2).mk)
CLEAN_MKS += clean/$(1)/$(2)
endef
$(foreach o,$(OBJECTS),$(eval $(call clean_mks,$(OMK),$(o))))
$(foreach t,$(TARGETS),$(eval $(call clean_mks,$(BMK),$(t))))

show/l:
	@echo $(LIBS)

show/p:
	@echo $(PROGS)

show/t:
	@echo $(TESTS)

show/rt:
	@echo $(REGULAR_TESTS)

show/dt:
	@echo $(DEATH_TESTS)

$(CREATE_PATHS):
	$(MKDIR_P) $@

define omks
$(BUILD)/$(1)/$(OMK)/%.mk: $(2)/%.h | $(BUILD)/$(1)/$(OMK)/
	@set -e; echo "Make object makefile $$@"; rm -f $$@; \
	$$(CC) -MM $$(CPPFLAGS) $$(CFLAGS) $$(<:.h=.c) > $$@.$$$$$$$$; \
	sed 's|.*:|$$(subst /$(OMK)/,/$(OBJ)/,$$(@:.mk=.o)):| ; $$$$s|$$$$| \| \
	  $(BUILD)/$(1)/$(OBJ)/| ; s| /[^ ]*| |g' < $$@.$$$$$$$$ > $$@; \
	$(ECHO_E) '\t$$$$(CC) $$$$(CPPFLAGS) $$$$(CFLAGS) $$$$(CFLAGS/$(1)) \
	  -c $$$$< -o $$$$@\n' >> $$@; \
	sed 's| /[^ ]*| |g ; s|.*:|$$@:| ; s|: [^ ]* |: |' < $$@.$$$$$$$$ >> $$@; \
	rm -f $$@.$$$$$$$$
endef
$(foreach m,$(MODES),$(foreach d,$(SRC_DIRS),$(eval $(call omks,$(m),$(d)))))
iomk = -include $(BUILD)/$(1)/$(OMK)/$(2).mk
ifdef INCLUDES
$(foreach m,$(MODES),$(foreach t,$(OBJECTS),$(eval $(call iomk,$(m),$(t)))))
endif

define bmks
$(BUILD)/$(1)/$(BMK)/$(2).mk: $(BUILD)/$(1)/$(OMK)/$(2).mk \
  | $(BUILD)/$(1)/$(BMK)/
	@set -e; echo "Make binary makefile $$@"; rm -f $$@; \
	sed '1s|$(OBJ)|$(BIN)| ; s|\.o|| ; s|: .*\.c |: | ; s|\.h|.o|g ; \
	  s^| $(BUILD)/$(1)/$(OBJ)/^--^ ; s| [^ ]*/| $(BUILD)/$(1)/$(OBJ)/|g ; \
	  s^--^| $(BUILD)/$(1)/$(BIN)/^ ; /\t/,$$$$d ; s|:|: $$@|' $$< > $$@; \
	$(ECHO_E) '\t$$$$(CC) $$$$(CPPFLAGS) $$$$(LDFLAGS) \c' >> $$@; \
	if grep -q $(GZ_FILE) $(BUILD)/$(1)/$(OMK)/$(2).mk; then \
	  $(ECHO_E) "-lz \c" >> $$@; \
	fi; \
	if grep -q $(PNG_FILE) $(BUILD)/$(1)/$(OMK)/$(2).mk; then \
	  $(ECHO_E) "-lpng \c" >> $$@; \
	fi; \
	if grep -q $(JPEG_FILE) $(BUILD)/$(1)/$(OMK)/$(2).mk; then \
	  $(ECHO_E) "-ljpeg \c" >> $$@; \
	fi; \
	$(ECHO_E) '$$$$(LDFLAGS/$(1)) $$$$(filter %.o,$$$$^) -o $$$$@$(3)\n' >> $$@
endef
$(foreach m,$(MODES),$(foreach p,$(TARGETS),$(eval $(call bmks,$(m),$(p), \
  $(POST_LD/$(m))))))
ibmk = -include $(BUILD)/$(1)/$(BMK)/$(2).mk
ifdef INCLUDES
$(foreach m,$(MODES),$(foreach t,$(TARGETS),$(eval $(call ibmk,$(m),$(t)))))
endif

define targets
$(1)/$(2): $(BUILD)/$(3)/$(BIN)/$(2)
$(4)TARGETS += $(1)/$(2)
endef
$(foreach t,$(TARGETS),$(eval $(call targets,d,$(t),$(DBG),D)))
$(foreach t,$(TARGETS),$(eval $(call targets,o,$(t),$(OPT),O)))
ATARGETS = $(DTARGETS) $(OTARGETS)

d/all: $(DTARGETS)
o/all: $(OTARGETS)
a/all: $(ATARGETS)
all: a/all

define tests
$(1)/$(2): $(BUILD)/$($(3))/$(BIN)/$(2)
	$(4)$(BUILD)/$($(3))/$(BIN)/$(2)
EXECUTE_TESTS_$(3) += $(1)/$(2)
endef
$(foreach t,$(REGULAR_TESTS),$(eval $(call tests,t,$(t),DBG,$(LSAN)1; )))
$(foreach t,$(DEATH_TESTS),$(eval $(call tests,t,$(t),DBG,$(LSAN)0; )))
$(foreach t,$(TESTS),$(eval $(call tests,ot,$(t),OPT)))

test: $(EXECUTE_TESTS_DBG)
otest: $(EXECUTE_TESTS_OPT)

define dmks
$(DIST)/$(DMK)/$(1).mk: $(BUILD)/$(OPT)/$(OMK)/$(1).mk | $(DIST)/$(DMK)/ \
  $(DIST)/$(TARGZ)
	@set -e; echo "Make distribution makefile $$@"; rm -f $$@; \
	sed 's|$(BUILD)/$(OPT)/$(OBJ)/$(1).o|$(DIST)/$(1)| ; \
	  s|$(BUILD)/$(OPT)/$(OMK)/$(1).mk|$$@| ; \
	  s|\t.*$$$$|\t$(RM_F) $(DIST)/$(SRC)/$(1)/$(LIB)/*.c \
	  $(DIST)/$(SRC)/$(1)/$(LIB)/*.h| ; \
	  s|^$$$$|\t$$(CP) $(DIST_DOCS) $(DIST)/$(SRC)/$(1)/\n\t$$(CP) \
	    $$$$(filter $(2)/%,$$$$^) $(DIST)/$(SRC)/$(1)/$(2)/\n\t$$(CP) \
	    $$$$(filter $(LIB)/%,$$$$^) $$$$(filter $(LIB)/%,$$$$(^:.h=.c)) \
	    $(DIST)/$(SRC)/$(1)/$(LIB)/\n\t$(FIND) $(DIST)/$(SRC) $(TOUCH) \
	    \n\t$(TAR_CZF) $(DIST)/$(TARGZ)/$(1).tar.gz -C \
	    $(DIST)/$(SRC)/$(1)/ .\n| ; \
	  s^| $(BUILD)/$(OPT)/$(OBJ)/^| $(DIST)/$(SRC)/$(1)/$(LIB)/ \
	    $(DIST)/$(SRC)/$(1)/$(PROG) $(DIST)/$(SRC)/$(1)/$(TEST)/^ \
	  ' $$< > $$@;
ifdef INCLUDES
-include $(DIST)/$(DMK)/$(1).mk
endif
endef
$(foreach p,$(PROGS),$(eval $(call dmks,$(p),$(PROG))))
$(foreach t,$(TESTS),$(eval $(call dmks,$(t),$(TEST))))

define rmks
$(REPO)/$(RMK)/$(1).mk: $(REPOS_MK) | $(REPO)/$(RMK)/ $(REPO)/$(TARGZ)
	@set -e; echo "Make repository makefile $$@"; rm -f $$@; \
	$(ECHO_E) '$(REPO)/$(1): $(REPO)/$(RMK)/$(1).mk | \
	  $(foreach d,$($(1)),$(DIST)/$(d)) \
	  $(REPO)/$(SRC)/$(1)/$(LIB)/ $(REPO)/$(SRC)/$(1)/$(PROG)/ \
	  $(REPO)/$(SRC)/$(1)/$(TEST)/ \
	\n\t$(RM_F) $(REPO)/$(SRC)/$(1)/$(LIB)/* $(REPO)/$(SRC)/$(1)/$(PROG)/* \
	  $(REPO)/$(SRC)/$(1)/$(TEST)/* \
	\n\t$(RM_RF) $(REPO)/$(SRC)/$(1)/$(BUILD)/ \
	  $(REPO)/$(SRC)/$(1)/$(DIST)/\
	\n\t$(CP) $(DIST_DOCS) $(REPO)/$(SRC)/$(1)/ \
	\n\t$(CP_R) $(foreach d,$($(1)),$(DIST)/$(SRC)/$(d)/$(LIB)/) \
	  $(REPO)/$(SRC)/$(1)/$(LIB)/ \
	\n\t$(CP_R) $(foreach d,$($(1)),$(DIST)/$(SRC)/$(d)/$(PROG)/) \
	  $(REPO)/$(SRC)/$(1)/$(PROG)/ \
	\n\t$(CP_R) $(foreach d,$($(1)),$(DIST)/$(SRC)/$(d)/$(TEST)/) \
	  $(REPO)/$(SRC)/$(1)/$(TEST)/ \
	\n\t$(TAR_CZF) $(REPO)/$(TARGZ)/$(1).tar.gz \
	  -C $(REPO)/$(SRC)/$(1)/ . \
	  ' > $(REPO)/$(RMK)/$(1).mk
ifdef INCLUDES
-include $(REPO)/$(RMK)/$(1).mk
endif
endef
$(foreach r,$(REPOS),$(eval $(call rmks,$(r))))


.PHONY: default all check clean%  $(DTARGETS) $(OTARGETS) \
  d/all o/all a/all show/l show/p show/t test otest dist/% repo/%  \
  $(EXECUTE_TESTS_DBG) $(EXECUTE_TESTS_OPT)
