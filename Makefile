MAKEFLAGS=--no-print-directory --quiet

ifeq ($(shell echo "check_quotes"),"check_quotes")
   WINDOWS := yes
else
   WINDOWS := no
endif

ifeq ($(WINDOWS),yes)
	path = "$(subst /,\,$(1))"
	exec = "$(call path,$(1)).exe"
	mkdir = mkdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	cp = -copy /y "$(1)" "$(2)"
	rm = $(wordlist 2,65535,$(foreach FILE,$(subst /,\,$(1)),& del $(FILE) > nul 2>&1)) || (exit 0)
	rmdir = rmdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	cd = chdir "$(call path,$(1))"
	echo = echo $(1)
else
	path = "$(1)"
	exec = "./$(1)"
	mkdir = mkdir -p $(1)
	cp = -cp -Rf "$(1)" "$(2)" > /dev/null 2>&1 || true
	rm = rm $(1) > /dev/null 2>&1 || true
	rmdir = rmdir $(1) > /dev/null 2>&1 || true
	cd = cd "$(1)"
	echo = echo "$(1)"
endif

PROJECT_NAME:=bf
OUTPUT_DIR:=bin
EXE=$(PROJECT_NAME)

BUILD:=build b
RUN:=run r

all: build run

$(BUILD): $(OUTPUT_DIR)
	echo ----- Building -----
	$(call mkdir,$(OUTPUT_DIR))
	$(call echo,main.c ...)
	gcc $(call path,src/main.c) -o $(call path,$(OUTPUT_DIR)/$(EXE))

$(RUN): build
	echo ----- Running -----
	-$(call cp,"assets","$(OUTPUT_DIR)")
	cd "$(OUTPUT_DIR)" && "$(call exec,$(EXE))" $(args)

$(OUTPUT_DIR):
	mkdir -p "$(OUTPUT_DIR)"

clean:
	git clean -Xdfq

.PHONY=$(BUILD) $(RUN) clean
