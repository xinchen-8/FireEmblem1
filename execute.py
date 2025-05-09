import subprocess

execute_path = r".\build\FireEmblem1.exe"

with open("output.log", "w") as fp:
    process = subprocess.Popen(
        [execute_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    fp.write("standard output:\n")
    fp.flush()
    for line in process.stdout:
        print(line, end="")
        fp.write(line)
        fp.flush() 

    fp.write("\nstd error:\n")
    fp.flush()
    for line in process.stderr:
        print(line, end="") 
        fp.write(line) 
        fp.flush()

    process.wait()
    fp.write(f"\nreturn code: {process.returncode}\n")
    fp.flush() 
