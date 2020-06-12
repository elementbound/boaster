build: compile

test: compile

compile: _ensure_out

_ensure_out:
	mkdir -p out

clean:
	rm -rf out
