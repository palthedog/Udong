UDONG_ROOT=$(git rev-parse --show-toplevel)

cd $UDONG_ROOT

# TODO: Make it easy to install all requirements (e.g. protoc)
#   then, generate .ts/.c at compile time.

echo 'Building .ts proto...'
protoc -I ./proto --plugin=protoc-gen-ts=${UDONG_ROOT}/web_conf/node_modules/.bin/protoc-gen-ts --ts_out=./web_conf/src/proto/ --ts_opt=explicit_override ./proto/config.proto

echo 'Building .c proto...'
${UDONG_ROOT}/firmware/lib/nanopb/generator/nanopb_generator.py --cpp-descriptors -I proto -D ./firmware/src/proto config.proto
