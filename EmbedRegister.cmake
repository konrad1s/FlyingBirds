file(APPEND "${OUT_CPP}" "
#include \"EmbeddedAssetRegistry.h\"

namespace
{
   struct EmbedRegister_${ASSET_OUT_NAME}
   {
       EmbedRegister_${ASSET_OUT_NAME}()
       {
           EmbeddedAssetRegistry::instance().registerAsset(
               \"${REL_PATH}\",
               ${ASSET_OUT_NAME},
               ${ASSET_OUT_NAME}_len
           );
       }
   } embedRegister_${ASSET_OUT_NAME};
}
")
