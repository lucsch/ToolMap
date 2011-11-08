# BUILD SCRIPT FOR CREATING TOOLMAP BINARY ON EVERY PLATEFORM
# Python 3.0 IS REQUIRED
# (c) Lucien Schreiber 2010
import shutil
import os.path
import sys


# script info
print ("\n----------------------------------------------------------")
print ("BUILD SCRIPT FOR CREATING TOOLMAP BINARY ON EVERY PLATEFORM")
print ("----------------------------------------------------------\n")

#
# FUNCTIONS
#
def askUserWithCheck (msg, allowedval=['Y', 'y', 'N', 'n'], errormsg = "Unsupported value"):
    "Ask user for entering a value with validy check, looping until correct value is entered"
    while(1):
        myValue = input(msg)
        if (myValue.isdigit()):
            myValue = int(myValue)
        if (myValue in allowedval):
            if(type(myValue) != type(int())):
                return myValue.upper()
            return myValue
        print (errormsg)

   
def runTests (msg, directory):
    "Run ctest and display error log if an error occur"
    print ("Running " + msg + " TEST in " + directory)
    myRetcode = 0
    try:
        myProcess = subprocess.Popen(['ctest', '--build-config', 'Debug'], 0, None, None, None,  None, None, False, False, directory)
        myRetcode = myProcess.wait()
    except:
        print ("Running "+ msg +" TEST  FAILED")
        return False
    if (myRetcode != 0):
        print ("Running "+ msg +" TEST  FAILED")
        print ("----------------------------------------------------------\n")
        myErrorFile = directory + os.sep + "Testing" + os.sep + "Temporary" + os.sep + "LastTest.log"
        try:
            readErrorFile = open(myErrorFile, 'r')
        except:
            print("Error reading log : " + myErrorFile)
            return False
        icount = 1
        for line in readErrorFile:
            print (str(icount) + " " + line, end='')
            icount = icount + 1
        print (readErrorFile.readlines())
        return False
    
    print ("Running "+ msg +" TEST DONE")
    #print ("retcode: " + str(myRetcode))
    print ("----------------------------------------------------------\n")
    return True

def CountProcessor():
    "Return the number of processor"
    import multiprocessing
    myNumberofProc = multiprocessing.cpu_count()
    return myNumberofProc
    

global NumberOfProc
NumberOfProc = CountProcessor()
    

#
# load variables possible variable files
#
myScriptPathName = os.path.split(__file__)
myVarsFile = os.listdir(myScriptPathName[0])

# copy the list before iterating... otherwise side effects.
myIterateList = myVarsFile[:]
for myFile in myIterateList:
    if (myFile.find(".py") <= 0 or myFile == myScriptPathName[1]):
        myVarsFile.remove (myFile)
icount = 1
for myFile in myVarsFile:
    print ("(",icount, ") -- ", myFile[:-3])
    icount = icount + 1

if (len(myVarsFile) == 0):
    print("no variable found, exiting")
    exit()
    
myvarAllowedValues = list(range(1,len(myVarsFile) + 2))
varChoose = askUserWithCheck("Select variable file number to use for build: ", myvarAllowedValues)
mySelectedVar = myVarsFile[varChoose-1]


try:
    exec(open(myScriptPathName[0] + os.sep + mySelectedVar).read())
except:
    print ("Using variables from ", mySelectedVar, "isn't possible!")
    print (myScriptPathName[0] + os.sep + mySelectedVar)
    exit(); 

#
# generic questions
#
doClean = askUserWithCheck("Clean directory before building ? (Y / N): ").upper()
doRelease = askUserWithCheck("Build release version ? (Y / N): ").upper()
doRunTests = askUserWithCheck("Run tests after build ? (Y/N): ").upper()
doInstall = askUserWithCheck("Create installer ? (not valid for Linux) (Y / N): ").upper()
doUpload = input("Upload Installer to FTP ? (Set password for Yes and leave empty for No) ")
print ("----------------------------------------------------------\n")

#
# display variables
#
print ("Using variables from :", mySelectedVar)
print ("wxWidgets :", gwxWidgets)
print ("Bin       :", gDirBin)
print ("Trunk     :", gDirTrunk)
print ("GDAL      :", gDirGdal)
print ("GEOS      :", gDirGeos)
print ("Unit test :", gDirUnitTest)
print ("CXX       :", gDirCxx)
print ("----------------------------------------------------------\n")
print ("Using ", NumberOfProc, " Processor(s)")
print ("----------------------------------------------------------\n")


doGo = askUserWithCheck ("Do the build ? (Y / N): ")


#
# get latest version
#
import subprocess
print ("Updating with latest version from Subversion")
try:
    myProcess = subprocess.Popen(['svn', 'update', gDirTrunk])
    myProcess.wait()
except:
    print ("Error updating SVN!")
    exit()
print ("Updating SVN DONE!")
print ("----------------------------------------------------------\n")


#
# Clean directory
#
if (doClean == 'Y' and gDirBin != ''):
    print ("Cleaning directory")
    try:
        shutil.rmtree(gDirBin)
        os.mkdir(gDirBin)
        print ("Cleaning directory DONE")
        print ("----------------------------------------------------------\n")
    except:
        print("Unable to remove directory :", gDirBin)

        
gBeforeConfig()

#
# Configure using CMAKE
#
mycmakeCommandLine = ['cmake']
mycmakeCommandLine.append("-G" + gCmakeEnv)
mycmakeCommandLine.append(gDirTrunk + os.sep + "build")
mycmakeCommandLine = mycmakeCommandLine + gCmakeSpecific
mycmakeCommandLine.append("-DSEARCH_GDAL:BOOL=1")
mycmakeCommandLine.append("-DSEARCH_GEOS:BOOL=1")
mycmakeCommandLine.append("-DSVN_DURING_BUILD:BOOL=1")
mycmakeCommandLine.append("-DSVN_DURING_CMAKE:BOOL=1")
mycmakeCommandLine.append("-DSEARCH_GDAL_LIB_PATH:PATH=" + gDirGdal)
mycmakeCommandLine.append("-DSEARCH_GEOS_LIB_PATH:PATH=" + gDirGeos)
mycmakeCommandLine.append("-DMYSQL_MAIN_DIR:PATH=" + gDirSQL)
mycmakeCommandLine.append("-DSEARCH_WXPDFDOCUMENT_PATH=" + gDirWXPDF)

if (gDirCurl):
    mycmakeCommandLine.append("-DSEARCH_CURL_LIB_PATH:PATH=" + gDirCurl)
if (doRunTests == 'Y'):
    mycmakeCommandLine.append("-DUSE_UNITTEST:BOOL=1")
else:
    mycmakeCommandLine.append("-DUSE_UNITTEST:BOOL=0")
mycmakeCommandLine.append("-DUNIT_TESTING_PATH:PATH=" + gDirUnitTest)
mycmakeCommandLine.append("-DCXXTEST_DIRECTORY:PATH=" + gDirCxx)
if(gwxWidgetsVersion):
	mycmakeCommandLine.append("-DwxWIDGETS_USING_SVN:BOOL=1")
else:
	mycmakeCommandLine.append("-DwxWIDGETS_USING_SVN:BOOL=0")
mycmakeCommandLine.append("-DwxWIDGETS_PATH_SVN:PATH=" + gwxWidgetsVersion)
	
	

print (" ".join(mycmakeCommandLine))
try:
    myProcess = subprocess.Popen(mycmakeCommandLine, 0, None, None, None,  None, None, False, False, gDirBin)
    myProcess.wait()
    if (doClean == 'Y'):
        print ("\n****Configuring again to be sure after a clean****")
        myProcess2 = subprocess.Popen(mycmakeCommandLine, 0, None, None, None,  None, None, False, False, gDirBin)
        myProcess2.wait()
except:
    print("Configuring project FAILED!")
    exit()
 
print ("Configuration DONE")
print ("----------------------------------------------------------\n")


if (doGo == 'N'):
    exit()
#
#building
#

print ("Building DEBUG ")
try:
    myProcess = subprocess.Popen(gBuildCommand("Debug",gDirBin), 0, None, None, None,  None, None, False, False, gDirBin)
    myProcess.wait()
except:
    print ("Building DEBUG version FAILED")
    exit()
print ("Building DEBUG DONE")
print ("----------------------------------------------------------\n")



if (doRelease == 'Y'):
    print ("Building RELEASE ")
    try:
        myProcess = subprocess.Popen(gBuildCommand("Release", gDirBin), 0, None, None, None,  None, None, False, False, gDirBin)
        myProcess.wait()
    except:
        print ("Building RELEASE version FAILED")
        exit()
    print ("Building RELEASE DONE")
    print ("----------------------------------------------------------\n")



#
# running all tests
#
if (doRunTests == 'Y'):
    runBeforeTest()
    if (not runTests("toolmap", gDirBin + os.sep + "test")):
        exit()

   
# Getting SVN number and creating installer
if (doInstall == 'Y'):
    with open(gDirBin + os.sep + "lsversion_param.h",  encoding='utf-8') as fileversion:
        for line in fileversion:
            if (line.find("lsVERSION_SOFT_VERSION") > 0):
                mySVNValue = line[line.find("\"")+1:line.rfind("\"")]
                print (mySVNValue)
                global installName
                installName = gCreateInstaller(mySVNValue)
                break;


#
# Upload to FTP
#

class ProgressFile(object):
    def __init__(self, file):
        self.file = file
        
    def read(self, size=None):
        data = self.file.read(size)
        print ('.', end='')
        sys.stdout.flush()
        return data


import ftplib as ftp
if (doUpload != '' and doInstall == 'Y'):
    myHost = "www.crealp.ch"
    myPath = "/htdocs/down/toolmap"
    myUser = "wwwcrealp"
    myConnection = ftp.FTP(myHost, myUser, doUpload)
    myConnection.cwd(myPath)
    myFileName = gDirInstall + os.sep + installName
    myConnection.storbinary('STOR '+installName, ProgressFile(open(myFileName, 'rb')), 4096)
    myConnection.quit()
    print ("transfering install " + installName + "Success!")
                    

        
    
    








