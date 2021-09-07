##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=dummy_project
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace
ProjectPath            :=C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fraun
Date                   :=07/09/2021
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="dummy_project.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/Testsuite.c$(ObjectSuffix) $(IntermediateDirectory)/System.c$(ObjectSuffix) $(IntermediateDirectory)/command_parser.c$(ObjectSuffix) $(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/time_keeper.c$(ObjectSuffix) $(IntermediateDirectory)/motor_controller.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(ConfigurationName)"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(ConfigurationName)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Testsuite.c$(ObjectSuffix): Testsuite.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Testsuite.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Testsuite.c$(DependSuffix) -MM Testsuite.c
	$(CC) $(SourceSwitch) "C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project/Testsuite.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Testsuite.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Testsuite.c$(PreprocessSuffix): Testsuite.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Testsuite.c$(PreprocessSuffix) Testsuite.c

$(IntermediateDirectory)/System.c$(ObjectSuffix): System.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/System.c$(ObjectSuffix) -MF$(IntermediateDirectory)/System.c$(DependSuffix) -MM System.c
	$(CC) $(SourceSwitch) "C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project/System.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/System.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/System.c$(PreprocessSuffix): System.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/System.c$(PreprocessSuffix) System.c

$(IntermediateDirectory)/command_parser.c$(ObjectSuffix): command_parser.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_parser.c$(ObjectSuffix) -MF$(IntermediateDirectory)/command_parser.c$(DependSuffix) -MM command_parser.c
	$(CC) $(SourceSwitch) "C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project/command_parser.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_parser.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_parser.c$(PreprocessSuffix): command_parser.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_parser.c$(PreprocessSuffix) command_parser.c

$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c
	$(CC) $(SourceSwitch) "C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

$(IntermediateDirectory)/time_keeper.c$(ObjectSuffix): time_keeper.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/time_keeper.c$(ObjectSuffix) -MF$(IntermediateDirectory)/time_keeper.c$(DependSuffix) -MM time_keeper.c
	$(CC) $(SourceSwitch) "C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project/time_keeper.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/time_keeper.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/time_keeper.c$(PreprocessSuffix): time_keeper.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/time_keeper.c$(PreprocessSuffix) time_keeper.c

$(IntermediateDirectory)/motor_controller.c$(ObjectSuffix): motor_controller.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/motor_controller.c$(ObjectSuffix) -MF$(IntermediateDirectory)/motor_controller.c$(DependSuffix) -MM motor_controller.c
	$(CC) $(SourceSwitch) "C:/Users/fraun/Documents/3D_druck/Sonstige_Moebel/curtain_closer/code/Window_curtain_alarm/proof_of_concept/dummy_closer_workspace/dummy_project/motor_controller.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/motor_controller.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/motor_controller.c$(PreprocessSuffix): motor_controller.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/motor_controller.c$(PreprocessSuffix) motor_controller.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


