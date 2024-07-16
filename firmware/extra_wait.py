import time

Import("env")

def wait_for_serial_port(source, target, env):
    print("Wait for a few seconds so that Serial port becomes ready")
    time.sleep(3)

if "test" in env.GetBuildType():
    env.AddPostAction(
        "upload",
        wait_for_serial_port)
