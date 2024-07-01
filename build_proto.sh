UDONG_ROOT=$(git rev-parse --show-toplevel)

cd $UDONG_ROOT

# TODO: Make it easy to install all requirements (e.g. protoc)
#   then, generate .ts/.c at compile time.

echo 'Building .ts proto...'
protoc -I ./proto --plugin=protoc-gen-ts=${UDONG_ROOT}/web_conf/node_modules/.bin/protoc-gen-ts --ts_out=./web_conf/src/proto/ --ts_opt=explicit_override --ts_opt=no_grpc --ts_opt=target=node ./proto/config.proto

echo 'Building .c proto...'
${UDONG_ROOT}/firmware/lib/nanopb/generator/nanopb_generator.py -s max_count:32 -s type:FT_STATIC --cpp-descriptors -I proto -D ./firmware/src/proto config.proto
#protoc -I ./proto --cpp_out=./firmware/src/proto config.proto