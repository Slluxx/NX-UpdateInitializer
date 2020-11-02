#include "main.h"

void checkForUpdates(Entry *localEntries, Entry *extEntries)
{
	Entry *tmpLocal = localEntries->next;
	Entry *tmpExt;

	u64 rawTime;
	TimeCalendarTime timeCal;
	TimeCalendarAdditionalInfo timeInfo;
	timeGetCurrentTime(TimeType_UserSystemClock, &rawTime);
	timeToCalendarTimeWithMyRule(rawTime, &timeCal, &timeInfo);

	nsInitialize();
	nifmInitialize(NifmServiceType_System);

	while (tmpLocal != NULL)
	{
		tmpExt = extEntries->next;
		while (tmpExt != NULL)
		{
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0)
			{
				if (tmpExt->Data.version > tmpLocal->Data.version)
				{

					/* Make network request. */
					NifmRequest request;
					if (R_SUCCEEDED(nifmCreateRequest(&request, true)))
					{
						nifmRequestSubmitAndWait(&request);

						/* Confirm network availability. */
						if (nifmIsAnyInternetRequestAccepted(nifmGetClientId()))
						{

							char textA[20];
							strcpy(textA, tmpLocal->Data.TID);
							char textB[6] = "000";
							int size = strlen(textA);
							textA[size - 3] = '\0';
							strcat(textA, textB);

							/* Iterate over installed applications. */
							s32 offset = 0, count = 0;
							NsApplicationRecord record;
							while (R_SUCCEEDED(nsListApplicationRecord(&record, 1, offset++, &count)) && count != 0)
							{
								/* Skip archived applications. */
								if (record.type == 0xb)
									continue;

								// long tid = atol(tmpLocal->Data.TID);
								// printf("%lu --> %lu \n", strtoul(tmpLocal->Data.TID, NULL, 16), record.application_id);
								// long int aid = *record.application_id;

								if (strtoul(textA, NULL, 16) == record.application_id)
								{
									printf("Found matching titleid for installed game: \n%s\n", tmpLocal->Data.name);

									AsyncResult async;
									if (R_SUCCEEDED(nsRequestUpdateApplication2(&async, record.application_id)))
									{

										/* Wait for result. */
										asyncResultWait(&async, UINT64_MAX);
										if (R_SUCCEEDED(asyncResultGet(&async)))
										{
											static NsApplicationControlData nacp;
											NacpLanguageEntry *entry = &nacp.nacp.lang[0];
											u64 size = 0;
											nsGetApplicationControlData(NsApplicationControlSource_Storage, record.application_id, &nacp, sizeof(nacp), &size);
											nacpGetLanguageEntry(&nacp.nacp, &entry);
											printf("\n->Updating: %s\n", entry->name);
										}
										else
										{
											printf("-> No update found\n");
										}
										asyncResultClose(&async);
									}
									else
									{
										printf("-> Update in progress\n");
									}
								}
							}
						}
						else
						{
							printf("Network connectivity couldn't be established.\nAborting!\n");
						}

						/* Close network request. */
						nifmRequestClose(&request);
					}
					else
					{
						printf("Failed to make a network request.\nAborting!\n");
					}
					printf("\n");
				}
				tmpExt = NULL;
			}
			else
			{
				tmpExt = tmpExt->next;
			}
		}
		tmpLocal = tmpLocal->next;
	}
	nifmExit();
	nsExit();
	return;
}