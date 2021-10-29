Import("env")

before_arg = env.GetProjectOption("before")
after_arg = env.GetProjectOption("after")

if before_arg == "":
        before_arg = "default_reset"

if after_arg == "":
        after_arg = "hard_reset"

env.Replace(
        UPLOADERFLAGS = ['--before', before_arg, '--after', after_arg, '--chip', 'esp8266', '--port', '"$UPLOAD_PORT"', '--baud', '$UPLOAD_SPEED', 'write_flash'],
        UPLOADER="/usr/bin/esptool",
        UPLOADCMD="$UPLOADER $UPLOADERFLAGS 0x0000 $SOURCE"
)