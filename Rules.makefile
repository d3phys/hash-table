#
# This file contains rules which are shared between multiple Makefiles.
# 2021, d3phys
#

#
# Special variables which should not be exported
#
unexport SUBDIRS
unexport OBJS

clean:
	@rm -f *.o
	@rm -f -R $(BUILD) 
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) clean); done

subdirs:
	@mkdir -p $(BUILD)
	@for i in $(SUBDIRS); do (cd $$i && echo $$i && $(MAKE)) || exit; done

#
# Dependencies 
#
dep:
	@sed '/\#\#\# Dependencies \#\#\#/q' < Makefile > temp_make
	@if [[ "$(wildcard *.cpp)" != "" ]]; then $(CPP) -MM *.cpp -I$(HPATH) >> temp_make; fi
	@cp temp_make Makefile
	@rm temp_make
	@for i in $(SUBDIRS); do (cd $$i && $(MAKE) dep) || exit; done

#
# Common rules
#
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $(addprefix -I, $(HPATH)) $< -o $(BUILD)/$@
