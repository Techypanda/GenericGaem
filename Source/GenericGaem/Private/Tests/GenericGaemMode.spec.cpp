// TODO: Invest in getting testing to work!

//#if WITH_AUTOMATION_TESTS
//#include "FWorldFixture.h"
//
//BEGIN_DEFINE_SPEC(FGenericGaemModeSpec, "GenericGaem.GenericGaemModeSpec", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext)
//TUniquePtr<FWorldFixture> WorldFixture;
//END_DEFINE_SPEC(FGenericGaemModeSpec)
//
//void FGenericGaemModeSpec::Define()
//{
//	Describe("FExampleFeature", [this]()
//		{
//			BeforeEach([this]()
//				{
//					WorldFixture = MakeUnique<FWorldFixture>();
//					if (TestNotNull("World", WorldFixture->GetWorld()))
//					{
//						// Spawn the actor
//						// DestructibleFixture = WorldFixture->GetWorld()->SpawnActor<AMyDestructible>();
//						// TestNotNull("MyDestructible", DestructibleFixture.Get());
//					}
//				});
//			AfterEach([this]()
//				{
//					// Tear down the world and the actor
//					WorldFixture.Reset();
//				});
//			It("Should do somethng", [this]() {
//				// FExampleFeature Feature;
//				TestTrue("MakeSomethingHappen", false);
//			});
//		});
//}
//#endif