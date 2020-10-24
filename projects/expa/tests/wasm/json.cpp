#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wasm/json.h>

#define GOS_EXPA_WASM_MESSAGE_TYPE "{\"t\":\"update\",\"m\":null,\"s\":null,\"u\":{\"t\":731,\"v\":{\"p\":[{\"x\":85.6874179731682,\"y\":0.4675405737174441},{\"x\":185.68741797316818,\"y\":8.694688058072833},{\"x\":285.6874179731682,\"y\":16.808514250854802}]}}}"

namespace gos {
namespace expa {
namespace wasm {

TEST(GosExpaWasmJsonTest, MessageType) {
  cJSON* message;
  GosWsMessageType type;

  message = cJSON_ParseWithLength(
    GOS_EXPA_WASM_MESSAGE_TYPE,
    sizeof(GOS_EXPA_WASM_MESSAGE_TYPE));
  EXPECT_FALSE(message == NULL);

  type = gos_json_get_message_type(message);
  EXPECT_EQ(GosWsMsgUpdate, type);
}

}
}
}
