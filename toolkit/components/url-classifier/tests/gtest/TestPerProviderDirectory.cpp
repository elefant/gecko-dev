#include "LookupCache.h"
#include "HashStore.h"
#include "gtest/gtest.h"
#include "nsIThread.h"
#include "nsAppDirectoryServiceDefs.h"
#include "nsThreadUtils.h"

namespace mozilla {
namespace safebrowsing {

class PerProviderDirectoryTestUtils {
public:
  template<typename T>
  static nsIFile* InspectStoreDirectory(const T& aT)
  {
    return aT.mStoreDirectory;
  }
};

} // end of namespace safebrowsing
} // end of namespace mozilla

using namespace mozilla;
using namespace mozilla::safebrowsing;

template<typename Function>
void RunTestInNewThread(Function&& aFunction) {
  nsCOMPtr<nsIRunnable> r = NS_NewRunnableFunction(mozilla::Forward<Function>(aFunction));
  nsCOMPtr<nsIThread> testingThread;
  nsresult rv = NS_NewThread(getter_AddRefs(testingThread), r);
  ASSERT_EQ(rv, NS_OK);
  testingThread->Shutdown();
}

template<typename T>
void VerifyPrivateStorePath(const char* aTableName,
                            const char* aProvider,
                            nsIFile* aRootDir,
                            bool aUsePerProviderStore)
{
  nsString rootStorePath;
  nsresult rv = aRootDir->GetPath(rootStorePath);
  EXPECT_EQ(rv, NS_OK);

  T target(nsCString(aTableName), aRootDir);

  nsIFile* privateStoreDirectory =
    PerProviderDirectoryTestUtils::InspectStoreDirectory(target);

  nsString privateStorePath;
  rv = privateStoreDirectory->GetPath(privateStorePath);
  ASSERT_EQ(rv, NS_OK);

  nsString expectedPrivateStorePath = rootStorePath;
  if (aUsePerProviderStore) {
    expectedPrivateStorePath.AppendPrintf("/%s", aProvider);
  }
  ASSERT_TRUE(privateStorePath == expectedPrivateStorePath);
}

TEST(PerProviderDirectory, LookupCache)
{
  RunTestInNewThread([] () -> void {
    nsCOMPtr<nsIFile> rootDir;
    NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR, getter_AddRefs(rootDir));

    // For V2 tables (NOT ending with '-proto'), root directory should be
    // used as the private store.
    VerifyPrivateStorePath<LookupCache>("goog-phish-shavar", "google", rootDir, false);

    // For V4 tables, if provider is found, use per-provider subdirectory;
    // If not found, use root directory.
    VerifyPrivateStorePath<LookupCache>("goog-noprovider-proto", "", rootDir, false);
    VerifyPrivateStorePath<LookupCache>("goog-phish-proto", "google4", rootDir, true);
  });
}

TEST(PerProviderDirectory, HashStore)
{
  RunTestInNewThread([] () -> void {
    nsCOMPtr<nsIFile> rootDir;
    NS_GetSpecialDirectory(NS_APP_USER_PROFILE_50_DIR, getter_AddRefs(rootDir));

    // For V2 tables (NOT ending with '-proto'), root directory should be
    // used as the private store.
    VerifyPrivateStorePath<HashStore>("goog-phish-shavar", "google", rootDir, false);

    // For V4 tables, if provider is found, use per-provider subdirectory;
    // If not found, use root directory.
    VerifyPrivateStorePath<HashStore>("goog-noprovider-proto", "", rootDir, false);
    VerifyPrivateStorePath<HashStore>("goog-phish-proto", "google4", rootDir, true);
  });
}
