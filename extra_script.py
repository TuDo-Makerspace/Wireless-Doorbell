Import("env", "projenv")

from genericpath import isfile
import os

from platformio.project.config import ProjectConfig

config = env.GetProjectConfig()

if config.has_option("env", "pwd_file"):
        pwd_file_arg = env.GetProjectOption("pwd_file")

        if not os.path.isfile(pwd_file_arg):
                raise Exception("Password file specified in platformio.ini not found: {}".format(pwd_file_arg))
        
        f = open(pwd_file_arg, "r")
        pwd = f.readline()
else:
        pwd = ""
        
projenv.Append(CPPDEFINES=("AP_PWD", "\\\"" + str(pwd) + "\\\""))
projenv.Append(CPPDEFINES=("HOST_AP_PWD", "\\\"" + str(pwd) + "\\\""))
projenv.Append(CPPDEFINES=("SCAN_AP_PWD", "\\\"" + str(pwd) + "\\\""))