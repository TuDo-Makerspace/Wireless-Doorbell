Import("env", "projenv")
from platformio.project.config import ProjectConfig

before_arg = env.GetProjectOption("before")
after_arg = env.GetProjectOption("after")

config = env.GetProjectConfig()

if config.has_option("env", "pwd_file"):
        pwd_file_arg = env.GetProjectOption("pwd_file")
        f = open(pwd_file_arg, "r")
        pwd = f.readline()
        projenv.Append(CPPDEFINES=("AP_PWD", '"\\"' + str(pwd) + '\\""'))

if before_arg == "":
        before_arg = "default_reset"

if after_arg == "":
        after_arg = "hard_reset"

env.Replace(
        UPLOADERFLAGS = ['--before', before_arg, '--after', after_arg, '--chip', 'esp8266', '--port', '"$UPLOAD_PORT"', '--baud', '$UPLOAD_SPEED', 'write_flash'],
        UPLOADER="/usr/bin/esptool",
        UPLOADCMD="$UPLOADER $UPLOADERFLAGS 0x0000 $SOURCE"
)