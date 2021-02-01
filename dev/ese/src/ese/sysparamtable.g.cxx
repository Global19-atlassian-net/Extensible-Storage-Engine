// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.


JetParam g_rgparamRaw[] =
{
    NORMAL_PARAM(JET_paramSystemPath, CJetParam::typeFolder, 0,  0,  0, 1, 0, 246, L".\\"),
    NORMAL_PARAM(JET_paramTempPath, CJetParam::typePath, 0,  0,  0, 1, 0, 247, L"tmp.edb"),
    NORMAL_PARAM(JET_paramLogFilePath, CJetParam::typeFolder, 0,  0,  0, 1, 0, 246, L".\\"),
    NORMAL_PARAM(JET_paramBaseName, CJetParam::typeString, 0,  0,  0, 1, 3, 3, L"edb"),
    NORMAL_PARAM(JET_paramEventSource, CJetParam::typeString, 0,  0,  0, 1, 0, 259, L""),
    NORMAL_PARAM2(JET_paramMaxSessions, CJetParam::typeInteger, 1,  0,  0, 1, 0, 30000, 30000, 16),
    NORMAL_PARAM2(JET_paramMaxOpenTables, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 2147483647, 300),
    IGNORED_PARAM(JET_paramPreferredMaxOpenTables, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 0),
    NORMAL_PARAM2(JET_paramMaxCursors, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 2147483647, 1024),
    NORMAL_PARAM2(JET_paramMaxVerPages, CJetParam::typeInteger, 1,  0,  0, 1, 1, 2147483647, 2147483647, 64),
    NORMAL_PARAM2(JET_paramMaxTemporaryTables, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 2147483647, 20),
    NORMAL_PARAM2(JET_paramLogFileSize, CJetParam::typeInteger, 0,  0,  0, 1, 64, 32768, 64, 5120),
    NORMAL_PARAM2(JET_paramLogBuffers, CJetParam::typeInteger, 1,  0,  0, 1, 1, 2147483647, 1, 126),
    IGNORED_PARAM(JET_paramWaitLogFlush, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2147483647, 0),
    IGNORED_PARAM(JET_paramLogCheckpointPeriod, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 1024),
    IGNORED_PARAM(JET_paramLogWaitingUserMax, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 3),
    CUSTOM_PARAM2(JET_paramCommitDefault, CJetParam::typeGrbit, 0,  0,  0, 0, NO_GRBIT, ULONG_PTR( ~NO_GRBIT ), NO_GRBIT, NO_GRBIT, GetCommitDefault, SetCommitDefault, CloneCommitDefault),
    NORMAL_PARAM(JET_paramCircularLog, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM2(JET_paramDbExtensionSize, CJetParam::typeInteger, 1,  0,  0, 0, 1, 2147483647, 16, 256),
    NORMAL_PARAM2(JET_paramPageTempDBMin, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 14, 0),
    NORMAL_PARAM(JET_paramPageFragment, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2147483647, 8),
    IGNORED_PARAM(JET_paramPageReadAheadMax, CJetParam::typeInteger, 1,  0,  0, 0, 0, -1, -1),
    IGNORED_PARAM(JET_paramBatchIOBufferMax, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 256),
    CUSTOM_PARAM3(JET_paramCacheSizeMax, CJetParam::typeInteger, 1,  1,  0, 0, 1, -1, 16, 2000000000, CJetParam::GetInteger, SetCacheSizeRange, CJetParam::CloneDefault),
    CUSTOM_PARAM3(JET_paramCheckpointDepthMax, CJetParam::typeInteger, 0,  0,  0, 0, 0, -1, 65536, 20971520, CJetParam::GetInteger, SetCheckpointDepthMax, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramLRUKCorrInterval, CJetParam::typeInteger, 1,  1,  1, 1, 0, -1, 128000),
    NORMAL_PARAM2(JET_paramLRUKHistoryMax, CJetParam::typeInteger, 1,  1,  1, 1, 0, -1, 10, 100000),
    NORMAL_PARAM(JET_paramLRUKPolicy, CJetParam::typeInteger, 1,  1,  1, 1, 1, 2, 2),
    NORMAL_PARAM(JET_paramLRUKTimeout, CJetParam::typeInteger, 1,  1,  1, 1, 1, -1, 100),
    IGNORED_PARAM(JET_paramLRUKTrxCorrInterval, CJetParam::typeInteger, 1,  1,  1, 1, 0, -1, 0),
    NORMAL_PARAM2(JET_paramOutstandingIOMax, CJetParam::typeInteger, 1,  1,  1, 1, 0, 65536, 16, 1024),
    NORMAL_PARAM2(JET_paramStartFlushThreshold, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 1, 20000000),
    NORMAL_PARAM2(JET_paramStopFlushThreshold, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 2, 40000000),
    IGNORED_PARAM(JET_paramTableClassName, CJetParam::typeString, 0,  1,  0, 0, 0, JET_cbNameMost, L""),
    NORMAL_PARAM(JET_paramRecovery, CJetParam::typeString, 0,  0,  0, 1, 0, 259, L"on"),
    NORMAL_PARAM(JET_paramEnableOnlineDefrag, CJetParam::typeGrbit, 0,  0,  0, 0, 0, JET_OnlineDefragAll, JET_OnlineDefragAll),
    NORMAL_PARAM(JET_paramAssertAction, CJetParam::typeGrbit, 0,  1,  0, 0, 0, 64, JET_AssertFailFast),
    READONLY_PARAM(JET_paramPrintFunction, CJetParam::typePointer, DBGFPrintF),
    CUSTOM_PARAM(JET_paramTransactionLevel, CJetParam::typeInteger, 0,  0,  0, 0, GetTransactionLevel, CJetParam::IllegalSet, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramRFS2IOsPermitted, CJetParam::typeInteger, 0,  1,  0, 0, 0, 0xfffffffe, 0),
    NORMAL_PARAM(JET_paramRFS2AllocsPermitted, CJetParam::typeInteger, 0,  1,  0, 0, 0, 0xfffffffe, 0),
    CUSTOM_PARAM(JET_paramCacheSize, CJetParam::typeInteger, 1,  1,  0, 0, GetCacheSize, SetCacheSize, CJetParam::CloneDefault),
    READONLY_PARAM(JET_paramCacheRequests, CJetParam::typeInteger, 1),
    READONLY_PARAM(JET_paramCacheHits, CJetParam::typeInteger, 1),
    IGNORED_PARAM(JET_paramCheckFormatWhenOpenFail, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 1),
    NORMAL_PARAM(JET_paramEnableIndexChecking, CJetParam::typeInteger, 0,  0,  0, 0, 0, JET_IndexCheckingMax - 1, 1),
    NORMAL_PARAM(JET_paramEnableTempTableVersioning, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 1),
    IGNORED_PARAM(JET_paramIgnoreLogVersion, CJetParam::typeBoolean, 0,  0,  0, 1, 0, 0, 0),
    IGNORED_PARAM(JET_paramDeleteOldLogs, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramEventSourceKey, CJetParam::typeString, 0,  0,  0, 1, 0, 259, L""),
    NORMAL_PARAM2(JET_paramNoInformationEvent, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 1, 0),
    NORMAL_PARAM(JET_paramEventLoggingLevel, CJetParam::typeInteger, 0,  0,  0, 1, JET_EventLoggingDisable, JET_EventLoggingLevelMax, JET_EventLoggingLevelMax),
    NORMAL_PARAM(JET_paramDeleteOutOfRangeLogs, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramAccessDeniedRetryPeriod, CJetParam::typeInteger, 0,  1,  0, 0, 0, -1, 10000),
    NORMAL_PARAM(JET_paramEnableIndexCleanup, CJetParam::typeBoolean, 0,  0,  0, 0, 0, -1, 1),
    NORMAL_PARAM(JET_paramFlight_SmoothIoTestPermillage, CJetParam::typeInteger, 1,  0,  0, 1, 0, 1000, 0),
    NORMAL_PARAM(JET_paramFlight_ElasticWaypointLatency, CJetParam::typeInteger, 1,  0,  0, 0, 0, 10, JET_paramFlight_ElasticWaypointLatency_DEFAULT),
    NORMAL_PARAM(JET_paramFlight_SynchronousLVCleanup, CJetParam::typeBoolean, 1,  0,  0, 0, 0, -1, 0),
    NORMAL_PARAM(JET_paramFlight_RBSRevertIOUrgentLevel, CJetParam::typeInteger, 1,  0,  0, 0, 0, 127, 8),
    NORMAL_PARAM(JET_paramFlight_EnableXpress10Compression, CJetParam::typeBoolean, 1,  0,  0, 0, 0, -1, 0),
    CUSTOM_PARAM3(JET_paramCacheSizeMin, CJetParam::typeInteger, 1,  1,  0, 0, 1, -1, 16, 1, CJetParam::GetInteger, SetCacheSizeRange, CJetParam::CloneDefault),
    IGNORED_PARAM(JET_paramLogFileFailoverPath, CJetParam::typeFolder, 0,  0,  0, 1, 0, 0, L""),
    IGNORED_PARAM(JET_paramEnableImprovedSeekShortcut, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM2(JET_paramPreferredVerPages, CJetParam::typeInteger, 1,  0,  0, 0, 1, 2147483647, 2147483647, 58),

    CUSTOM_PARAM3(JET_paramDatabasePageSize, CJetParam::typeBlockSize, 0,  1,  1, 1, g_cbPageMin, g_cbPageMax, g_cbPageDefault, g_cbPageDefault, CJetParam::GetInteger, SetDatabasePageSize, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramDisableCallbacks, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    IGNORED_PARAM(JET_paramBackupChunkSize, CJetParam::typeInteger, 1,  0,  0, 1, 1, 2147483647, 16),
    IGNORED_PARAM(JET_paramBackupOutstandingReads, CJetParam::typeInteger, 1,  0,  0, 1, 1, 2147483647, 8),
    ILLEGAL_PARAM(68),
    NORMAL_PARAM(JET_paramLogFileCreateAsynch, CJetParam::typeBoolean, 1,  0,  0, 1, 0, -1, fTrue),
    CUSTOM_PARAM(JET_paramErrorToString, CJetParam::typeUserDefined, 0,  1,  0, 0, GetErrorToString, CJetParam::IllegalSet, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramZeroDatabaseDuringBackup, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    CUSTOM_PARAM(JET_paramUnicodeIndexDefault, CJetParam::typeUserDefined, 0,  0,  0, 1, GetUnicodeIndexDefault, SetUnicodeIndexDefault, CloneUnicodeIndexDefault),
    NORMAL_PARAM(JET_paramRuntimeCallback, CJetParam::typePointer, 0,  0,  0, 1, 0, -1, NULL),
    NORMAL_PARAM(JET_paramFlight_EnableReattachRaceBugFix, CJetParam::typeBoolean, 1,  0,  0, 1, 0, -1, 1),
    ILLEGAL_PARAM(75),
    IGNORED_PARAM(JET_paramEnableSortedRetrieveColumns, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramCleanupMismatchedLogFiles, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramRecordUpgradeDirtyLevel, CJetParam::typeGrbit, 1,  1,  0, 0, 0, 3, 1),
    CUSTOM_PARAM(JET_paramRecoveryCurrentLogfile, CJetParam::typeInteger, 0,  0,  0, 0, GetRecoveryCurrentLogfile, CJetParam::IllegalSet, CJetParam::CloneDefault),
    ILLEGAL_PARAM(80),
    NORMAL_PARAM2(JET_paramGlobalMinVerPages, CJetParam::typeInteger, 1,  1,  1, 1, 1, 2147483647, 1, 64),
    NORMAL_PARAM(JET_paramOSSnapshotTimeout, CJetParam::typeInteger, 0,  1,  0, 0, 0, 2147483647, 70000),
    ILLEGAL_PARAM(83),
    NORMAL_PARAM(JET_paramFlight_NewQueueOptions, CJetParam::typeInteger, 0,  0,  0, 0, 0, bitNewQueueOptionsMask, grbitEseSkuQueueOptionDefault),
    NORMAL_PARAM(JET_paramFlight_ConcurrentMetedOps, CJetParam::typeInteger, 0,  0,  0, 1, 0, 1024, 2),
    NORMAL_PARAM(JET_paramFlight_LowMetedOpsThreshold, CJetParam::typeInteger, 0,  0,  0, 1, 0, 1024, 40),
    NORMAL_PARAM(JET_paramFlight_MetedOpStarvedThreshold, CJetParam::typeInteger, 0,  0,  0, 1, 50, 2147483647, 3000),
    ILLEGAL_PARAM(88),
    NORMAL_PARAM(JET_paramFlight_EnableShrinkArchiving, CJetParam::typeBoolean, 1,  0,  0, 0, 0, 1, 1),
    NORMAL_PARAM(JET_paramFlight_EnableBackupDuringRecovery, CJetParam::typeBoolean, 1,  0,  0, 0, 0, -1, 0),
    NORMAL_PARAM(JET_paramFlight_RBSRollIntervalSec, CJetParam::typeInteger, 1,  0,  0, 0, 0, 604800, 43200),
    NORMAL_PARAM(JET_paramFlight_RBSMaxRequiredRange, CJetParam::typeInteger, 1,  0,  0, 0, 0, 10000, 1000),
    NORMAL_PARAM(JET_paramFlight_RBSCleanupEnabled, CJetParam::typeBoolean, 1,  0,  0, 0, 0, 1, JET_paramFlight_RBSCleanupEnabledDEFAULT),
    NORMAL_PARAM(JET_paramFlight_RBSLowDiskSpaceThresholdGb, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2147483647, 100),
    NORMAL_PARAM(JET_paramFlight_RBSMaxSpaceWhenLowDiskSpaceGb, CJetParam::typeInteger, 1,  0,  0, 0, 0, 1000, 150),
    NORMAL_PARAM(JET_paramFlight_RBSMaxTimeSpanSec, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2147483647, 604800),
    NORMAL_PARAM(JET_paramFlight_RBSCleanupIntervalMinSec, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2147483647, 3600),
    NORMAL_PARAM(JET_paramExceptionAction, CJetParam::typeInteger, 0,  1,  0, 0, 0, -1, JET_ExceptionFailFast),
    NORMAL_PARAM(JET_paramEventLogCache, CJetParam::typeInteger, 0,  1,  1, 1, 0, 2147483647, 0),
    NORMAL_PARAM(JET_paramCreatePathIfNotExist, CJetParam::typeBoolean, 0,  0,  0, 0, 0, -1, 0),
    NORMAL_PARAM2(JET_paramPageHintCacheSize, CJetParam::typeInteger, 1,  1,  1, 1, 0, 2147483647, 32, 262144),
    NORMAL_PARAM(JET_paramOneDatabasePerSession, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    IGNORED_PARAM(JET_paramMaxDatabasesPerInstance, CJetParam::typeInteger, 0,  1,  1, 1, 7, 7, 7),
    NORMAL_PARAM(JET_paramMaxInstances, CJetParam::typeInteger, 0,  1,  1, 1, 1, cMaxInstances, 16),
    NORMAL_PARAM(JET_paramVersionStoreTaskQueueMax, CJetParam::typeInteger, 1,  0,  0, 0, 1, 127, 4),
    IGNORED_PARAM(JET_paramIdleFlushTime, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 0),
    NORMAL_PARAM2(JET_paramDisablePerfmon, CJetParam::typeBoolean, 0,  1,  1, 1, 0, -1, fTrue, JET_paramDisablePerfmon_DEFAULT),
    IGNORED_PARAM(JET_paramAbortRetryFailCallback, CJetParam::typePointer, 0,  1,  1, 1, NULL, NULL, 0),
    IGNORED_PARAM(JET_paramVAReserve, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 0),
    NORMAL_PARAM(JET_paramIndexTuplesLengthMin, CJetParam::typeInteger, 0,  0,  0, 1, 2, 255, 3),
    NORMAL_PARAM(JET_paramIndexTuplesLengthMax, CJetParam::typeInteger, 0,  0,  0, 1, 2, 255, 10),
    NORMAL_PARAM(JET_paramIndexTuplesToIndexMax, CJetParam::typeInteger, 0,  0,  0, 1, 1, 32767, 32767),
    NORMAL_PARAM(JET_paramAlternateDatabaseRecoveryPath, CJetParam::typeFolder, 0,  0,  0, 1, 0, 246, L""),
    ILLEGAL_PARAM(114),
    ILLEGAL_PARAM(115),
    ILLEGAL_PARAM(116),
    ILLEGAL_PARAM(117),
    ILLEGAL_PARAM(118),
    ILLEGAL_PARAM(119),
    IGNORED_PARAM(JET_paramDBAPageAvailMin, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 1280),
    CUSTOM_PARAM(JET_paramDBAPageAvailThreshold, CJetParam::typeUserDefined, 0,  1,  0, 0, CJetParam::IgnoreGet, CJetParam::IgnoreSet, CJetParam::CloneDefault),
    CUSTOM_PARAM(JET_paramDBAK1, CJetParam::typeUserDefined, 0,  1,  0, 0, CJetParam::IgnoreGet, CJetParam::IgnoreSet, CJetParam::CloneDefault),
    CUSTOM_PARAM(JET_paramDBAK2, CJetParam::typeUserDefined, 0,  1,  0, 0, CJetParam::IgnoreGet, CJetParam::IgnoreSet, CJetParam::CloneDefault),
    IGNORED_PARAM(JET_paramMaxRandomIOSize, CJetParam::typeInteger, 1,  1,  0, 0, 0, -1, 1024 * 1024),
    CUSTOM_PARAM3(JET_paramCachedClosedTables, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2147483647, 64, 64, CJetParam::GetInteger, ErrSetCacheClosedTables, CJetParam::CloneDefault),
    NORMAL_PARAM2(JET_paramEnableFileCache, CJetParam::typeBoolean, 1,  1,  1, 1, 0, -1, 1, 0),
    NORMAL_PARAM2(JET_paramEnableViewCache, CJetParam::typeBoolean, 1,  1,  1, 1, 0, -1, 1, 0),
    NORMAL_PARAM2(JET_paramVerPageSize, CJetParam::typeBlockSize, 1,  1,  1, 1, 1024, 65536, 8192, cbDefaultVerPageSize),
    CUSTOM_PARAM3(JET_paramConfiguration, CJetParam::typeInteger, 0,  0,  0, 1, ULONG_PTR( 0 ), JET_configMask, ULONG_PTR( JET_configDefault | JET_configLowMemory ), ULONG_PTR( JET_configDefault ), GetConfiguration, SetConfiguration, CloneConfiguration),
    NORMAL_PARAM2(JET_paramEnableAdvanced, CJetParam::typeBoolean, 0,  0,  0, 0, 0, -1, 1, 1),
    READONLY_PARAM(JET_paramMaxColtyp, CJetParam::typeInteger, JET_coltypMax),
    NORMAL_PARAM(JET_paramIndexTupleIncrement, CJetParam::typeInteger, 0,  0,  0, 1, 0, 32767, 1),
    NORMAL_PARAM(JET_paramIndexTupleStart, CJetParam::typeInteger, 0,  0,  0, 0, 0, 32767, 0),
    CUSTOM_PARAM(JET_paramKeyMost, CJetParam::typeInteger, 0,  0,  0, 0, GetKeyMostMost, CJetParam::IllegalSet, CJetParam::CloneDefault),
    NORMAL_PARAM2(JET_paramCheckpointIOMax, CJetParam::typeInteger, 1,  1,  0, 0, 1, 1024, 1, 1),
    NORMAL_PARAM(JET_paramLegacyFileNames, CJetParam::typeGrbit, 0,  0,  0, 1, 0, 255, JET_paramLegacyFileNames_DEFAULT),
    NORMAL_PARAM(JET_paramTableClass1Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass1"),
    NORMAL_PARAM(JET_paramTableClass2Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass2"),
    NORMAL_PARAM(JET_paramTableClass3Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass3"),
    NORMAL_PARAM(JET_paramTableClass4Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass4"),
    NORMAL_PARAM(JET_paramTableClass5Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass5"),
    NORMAL_PARAM(JET_paramTableClass6Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass6"),
    NORMAL_PARAM(JET_paramTableClass7Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass7"),
    NORMAL_PARAM(JET_paramTableClass8Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass8"),
    NORMAL_PARAM(JET_paramTableClass9Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass9"),
    NORMAL_PARAM(JET_paramTableClass10Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass10"),
    NORMAL_PARAM(JET_paramTableClass11Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass11"),
    NORMAL_PARAM(JET_paramTableClass12Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass12"),
    NORMAL_PARAM(JET_paramTableClass13Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass13"),
    NORMAL_PARAM(JET_paramTableClass14Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass14"),
    NORMAL_PARAM(JET_paramTableClass15Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass15"),
    NORMAL_PARAM(JET_paramIOPriority, CJetParam::typeInteger, 1,  0,  0, 0, 0, JET_IOPriorityMax, 0),
    NORMAL_PARAM(JET_paramWaypointLatency, CJetParam::typeInteger, 0,  0,  0, 1, 0, 1023, 0),
    NORMAL_PARAM(JET_paramCheckpointTooDeep, CJetParam::typeInteger, 0,  0,  0, 1, lgenCheckpointTooDeepMin, lgenCheckpointTooDeepMax, lgenCheckpointTooDeepMax),
    NORMAL_PARAM(JET_paramDisableBlockVerification, CJetParam::typeBoolean, 0,  1,  1, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramEnablePersistedCallbacks, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, JET_paramEnablePersistedCallbacks_DEFAULT),
    NORMAL_PARAM(JET_paramAggressiveLogRollover, CJetParam::typeBoolean, 0,  0,  0, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramPeriodicLogRolloverLLR, CJetParam::typeInteger, 0,  0,  0, 1, 0, 7*24*60*60, 0),
    IGNORED_PARAM(JET_paramUsePageDependencies, CJetParam::typeBoolean, 0,  1,  1, 1, 0, -1, 0),
    NORMAL_PARAM(JET_paramDefragmentSequentialBTrees, CJetParam::typeBoolean, 0,  0,  0, 0, 0, -1, 1),
    NORMAL_PARAM(JET_paramDefragmentSequentialBTreesDensityCheckFrequency, CJetParam::typeInteger, 0,  0,  0, 1, 0, -1, 10),
    NORMAL_PARAM(JET_paramIOThrottlingTimeQuanta, CJetParam::typeInteger, 0,  1,  1, 1, 1, 10000, 100),
    READONLY_PARAM(JET_paramLVChunkSizeMost, CJetParam::typeInteger, 1024 * 4 - LVChunkOverheadSmallPage),
    NORMAL_PARAM(JET_paramMaxCoalesceReadSize, CJetParam::typeInteger, 1,  1,  1, 1, 0, 10 * 1024 * 1024, cbReadSizeMax),
    NORMAL_PARAM(JET_paramMaxCoalesceWriteSize, CJetParam::typeInteger, 1,  1,  1, 1, 0, 10 * 1024 * 1024, cbWriteSizeMax),
    NORMAL_PARAM(JET_paramMaxCoalesceReadGapSize, CJetParam::typeInteger, 1,  1,  1, 1, 0, 10 * 1024 * 1024, cbReadSizeMax),
    NORMAL_PARAM(JET_paramMaxCoalesceWriteGapSize, CJetParam::typeInteger, 1,  1,  1, 1, 0, 10 * 1024 * 1024, cbWriteSizeMax),
    NORMAL_PARAM(JET_paramEnableHaPublish, CJetParam::typeInteger, 1,  0,  0, 1, 0, 2, 0),
    NORMAL_PARAM(JET_paramEnableDBScanInRecovery, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2, 0),
    NORMAL_PARAM(JET_paramDbScanThrottle, CJetParam::typeInteger, 1,  0,  0, 0, 0, 1000 * 60, 20),
    NORMAL_PARAM(JET_paramDbScanIntervalMinSec, CJetParam::typeInteger, 1,  0,  0, 1, 0, 60 * 60 * 24 * 21, 60 * 60 * 24 * 1),
    NORMAL_PARAM(JET_paramDbScanIntervalMaxSec, CJetParam::typeInteger, 1,  0,  0, 1, 1, 60 * 60 * 24 * 365 * 50, 60 * 60 * 24 * 7),
    NORMAL_PARAM(JET_paramEmitLogDataCallback, CJetParam::typePointer, 1,  0,  0, 1, 0, -1, NULL),
    NORMAL_PARAM(JET_paramEmitLogDataCallbackCtx, CJetParam::typePointer, 1,  0,  0, 1, 0, -1, NULL),
    NORMAL_PARAM(JET_paramEnableExternalAutoHealing, CJetParam::typeBoolean, 1,  0,  0, 1, 0, 1, 0),
    NORMAL_PARAM(JET_paramPatchRequestTimeout, CJetParam::typeInteger, 1,  0,  0, 0, 0, 2147483647, 300),
    CUSTOM_PARAM3(JET_paramCachePriority, CJetParam::typeInteger, 1,  0,  0, 0, g_pctCachePriorityMin, g_pctCachePriorityMax, g_pctCachePriorityDefault, g_pctCachePriorityDefault, CJetParam::GetInteger, SetCachePriority, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramMaxTransactionSize, CJetParam::typeInteger, 1,  0,  0, 0, 1, 100, 100),
    NORMAL_PARAM(JET_paramPrereadIOMax, CJetParam::typeInteger, 1,  0,  0, 0, 0, 256, 8),
    NORMAL_PARAM(JET_paramEnableDBScanSerialization, CJetParam::typeBoolean, 1,  0,  0, 1, 0, 1, 0),
    NORMAL_PARAM(JET_paramHungIOThreshold, CJetParam::typeInteger, 1,  1,  1, 1, 11*1000, 30*60*1000, 30*1000),  // min = 11 sec (see Reasonable Period in http:
    NORMAL_PARAM(JET_paramHungIOActions, CJetParam::typeInteger, 1,  1,  1, 1, JET_bitNil, (JET_bitHungIOEvent|JET_bitHungIOCancel|JET_bitHungIODebug|JET_bitHungIOEnforce|JET_bitHungIOTimeout), JET_bitHungIOEvent),
    NORMAL_PARAM(JET_paramMinDataForXpress, CJetParam::typeInteger, 1,  1,  1, 1, 0, 2147483647, 1024),
    CUSTOM_PARAM3(JET_paramEnableShrinkDatabase, CJetParam::typeGrbit, 0,  0,  0, 1, 0, 0xffff, JET_paramEnableShrinkDatabase_DEFAULT, JET_paramEnableShrinkDatabase_DEFAULT, CJetParam::GetInteger, SetShrinkDatabaseParam, CJetParam::CloneDefault),
    ILLEGAL_PARAM(185),
    NORMAL_PARAM(JET_paramProcessFriendlyName, CJetParam::typeString, 0,  1,  1, 1, 0, JET_cbNameMost, L""),
    NORMAL_PARAM(JET_paramDurableCommitCallback, CJetParam::typePointer, 1,  0,  0, 1, 0, -1, NULL),
    IGNORED_PARAM(JET_paramEnableSqm, CJetParam::typeInteger, 0,  0,  0, 1, JET_sqmDisable, JET_sqmFromCEIP, JET_sqmEnable),
    CUSTOM_PARAM(JET_paramConfigStoreSpec, CJetParam::typeString, 0,  0,  0, 1, CJetParam::GetString, ErrSetConfigStoreSpec, CJetParam::CloneString),
    NORMAL_PARAM(JET_paramStageFlighting, CJetParam::typeInteger, 1,  0,  0, 0, 0x0, ulMax, JET_paramStageFlighting_DEFAULT),
    NORMAL_PARAM(JET_paramZeroDatabaseUnusedSpace, CJetParam::typeBoolean, 1,  0,  0, 0, 0, 1, 0),
    NORMAL_PARAM(JET_paramDisableVerifications, CJetParam::typeBoolean, 1,  0,  0, 0, 0, 1, 0),
    NORMAL_PARAM(JET_paramPersistedLostFlushDetection, CJetParam::typeInteger, 1,  0,  0, 1, 0, 7, JET_paramPersistedLostFlushDetection_DEFAULT),

    CUSTOM_PARAM3(JET_paramEngineFormatVersion, CJetParam::typeInteger, 0,  0,  0, 0, 0, ulMax, JET_efvUseEngineDefault, JET_efvUseEngineDefault, ErrGetEngineFormatVersionParameter, ErrSetEngineFormatVersionParameter, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramReplayThrottlingLevel, CJetParam::typeInteger, 1,  0,  0, 0, JET_ReplayThrottlingNone, JET_ReplayThrottlingSleep, JET_ReplayThrottlingNone),
    NORMAL_PARAM(JET_paramTableClass16Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass16"),
    NORMAL_PARAM(JET_paramTableClass17Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass17"),
    NORMAL_PARAM(JET_paramTableClass18Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass18"),
    NORMAL_PARAM(JET_paramTableClass19Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass19"),
    NORMAL_PARAM(JET_paramTableClass20Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass20"),
    NORMAL_PARAM(JET_paramTableClass21Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass21"),
    NORMAL_PARAM(JET_paramTableClass22Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass22"),
    NORMAL_PARAM(JET_paramTableClass23Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass23"),
    NORMAL_PARAM(JET_paramTableClass24Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass24"),
    NORMAL_PARAM(JET_paramTableClass25Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass25"),
    NORMAL_PARAM(JET_paramTableClass26Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass26"),
    NORMAL_PARAM(JET_paramTableClass27Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass27"),
    NORMAL_PARAM(JET_paramTableClass28Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass28"),
    NORMAL_PARAM(JET_paramTableClass29Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass29"),
    NORMAL_PARAM(JET_paramTableClass30Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass30"),
    NORMAL_PARAM(JET_paramTableClass31Name, CJetParam::typeString, 0,  1,  1, 1, 1, JET_cbNameMost, L"TableClass31"),
    NORMAL_PARAM(JET_paramBlockCacheConfiguration, CJetParam::typePointer, 1,  0,  0, 1, 0, -1, NULL),
    CUSTOM_PARAM(JET_paramRecordSizeMost, CJetParam::typeInteger, 0,  1,  0, 0, GetRecordSizeMost, CJetParam::IllegalSet, CJetParam::CloneDefault),
    NORMAL_PARAM(JET_paramUseFlushForWriteDurability, CJetParam::typeBoolean, 1,  0,  0, 1, 0, 1, 1),
    NORMAL_PARAM(JET_paramEnableRBS, CJetParam::typeBoolean, 1,  0,  0, 0, 0, 1, 0),
    NORMAL_PARAM(JET_paramRBSFilePath, CJetParam::typeFolder, 0,  0,  0, 1, 0, 246, L".\\"),
    ILLEGAL_PARAM(JET_paramMaxValueInvalid),
};

    
static_assert( JET_paramSystemPath == 0, "The order of defintion for JET_paramSystemPath in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTempPath == 1, "The order of defintion for JET_paramTempPath in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogFilePath == 2, "The order of defintion for JET_paramLogFilePath in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramBaseName == 3, "The order of defintion for JET_paramBaseName in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEventSource == 4, "The order of defintion for JET_paramEventSource in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxSessions == 5, "The order of defintion for JET_paramMaxSessions in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxOpenTables == 6, "The order of defintion for JET_paramMaxOpenTables in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPreferredMaxOpenTables == 7, "The order of defintion for JET_paramPreferredMaxOpenTables in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxCursors == 8, "The order of defintion for JET_paramMaxCursors in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxVerPages == 9, "The order of defintion for JET_paramMaxVerPages in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxTemporaryTables == 10, "The order of defintion for JET_paramMaxTemporaryTables in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogFileSize == 11, "The order of defintion for JET_paramLogFileSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogBuffers == 12, "The order of defintion for JET_paramLogBuffers in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramWaitLogFlush == 13, "The order of defintion for JET_paramWaitLogFlush in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogCheckpointPeriod == 14, "The order of defintion for JET_paramLogCheckpointPeriod in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogWaitingUserMax == 15, "The order of defintion for JET_paramLogWaitingUserMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCommitDefault == 16, "The order of defintion for JET_paramCommitDefault in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCircularLog == 17, "The order of defintion for JET_paramCircularLog in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDbExtensionSize == 18, "The order of defintion for JET_paramDbExtensionSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPageTempDBMin == 19, "The order of defintion for JET_paramPageTempDBMin in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPageFragment == 20, "The order of defintion for JET_paramPageFragment in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPageReadAheadMax == 21, "The order of defintion for JET_paramPageReadAheadMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramBatchIOBufferMax == 22, "The order of defintion for JET_paramBatchIOBufferMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCacheSizeMax == 23, "The order of defintion for JET_paramCacheSizeMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCheckpointDepthMax == 24, "The order of defintion for JET_paramCheckpointDepthMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLRUKCorrInterval == 25, "The order of defintion for JET_paramLRUKCorrInterval in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLRUKHistoryMax == 26, "The order of defintion for JET_paramLRUKHistoryMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLRUKPolicy == 27, "The order of defintion for JET_paramLRUKPolicy in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLRUKTimeout == 28, "The order of defintion for JET_paramLRUKTimeout in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLRUKTrxCorrInterval == 29, "The order of defintion for JET_paramLRUKTrxCorrInterval in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramOutstandingIOMax == 30, "The order of defintion for JET_paramOutstandingIOMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramStartFlushThreshold == 31, "The order of defintion for JET_paramStartFlushThreshold in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramStopFlushThreshold == 32, "The order of defintion for JET_paramStopFlushThreshold in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClassName == 33, "The order of defintion for JET_paramTableClassName in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRecovery == 34, "The order of defintion for JET_paramRecovery in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableOnlineDefrag == 35, "The order of defintion for JET_paramEnableOnlineDefrag in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramAssertAction == 36, "The order of defintion for JET_paramAssertAction in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPrintFunction == 37, "The order of defintion for JET_paramPrintFunction in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTransactionLevel == 38, "The order of defintion for JET_paramTransactionLevel in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRFS2IOsPermitted == 39, "The order of defintion for JET_paramRFS2IOsPermitted in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRFS2AllocsPermitted == 40, "The order of defintion for JET_paramRFS2AllocsPermitted in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCacheSize == 41, "The order of defintion for JET_paramCacheSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCacheRequests == 42, "The order of defintion for JET_paramCacheRequests in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCacheHits == 43, "The order of defintion for JET_paramCacheHits in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCheckFormatWhenOpenFail == 44, "The order of defintion for JET_paramCheckFormatWhenOpenFail in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableIndexChecking == 45, "The order of defintion for JET_paramEnableIndexChecking in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableTempTableVersioning == 46, "The order of defintion for JET_paramEnableTempTableVersioning in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIgnoreLogVersion == 47, "The order of defintion for JET_paramIgnoreLogVersion in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDeleteOldLogs == 48, "The order of defintion for JET_paramDeleteOldLogs in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEventSourceKey == 49, "The order of defintion for JET_paramEventSourceKey in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramNoInformationEvent == 50, "The order of defintion for JET_paramNoInformationEvent in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEventLoggingLevel == 51, "The order of defintion for JET_paramEventLoggingLevel in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDeleteOutOfRangeLogs == 52, "The order of defintion for JET_paramDeleteOutOfRangeLogs in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramAccessDeniedRetryPeriod == 53, "The order of defintion for JET_paramAccessDeniedRetryPeriod in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableIndexCleanup == 54, "The order of defintion for JET_paramEnableIndexCleanup in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_SmoothIoTestPermillage == 55, "The order of defintion for JET_paramFlight_SmoothIoTestPermillage in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_ElasticWaypointLatency == 56, "The order of defintion for JET_paramFlight_ElasticWaypointLatency in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_SynchronousLVCleanup == 57, "The order of defintion for JET_paramFlight_SynchronousLVCleanup in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSRevertIOUrgentLevel == 58, "The order of defintion for JET_paramFlight_RBSRevertIOUrgentLevel in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_EnableXpress10Compression == 59, "The order of defintion for JET_paramFlight_EnableXpress10Compression in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCacheSizeMin == 60, "The order of defintion for JET_paramCacheSizeMin in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogFileFailoverPath == 61, "The order of defintion for JET_paramLogFileFailoverPath in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableImprovedSeekShortcut == 62, "The order of defintion for JET_paramEnableImprovedSeekShortcut in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPreferredVerPages == 63, "The order of defintion for JET_paramPreferredVerPages in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDatabasePageSize == 64, "The order of defintion for JET_paramDatabasePageSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDisableCallbacks == 65, "The order of defintion for JET_paramDisableCallbacks in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramBackupChunkSize == 66, "The order of defintion for JET_paramBackupChunkSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramBackupOutstandingReads == 67, "The order of defintion for JET_paramBackupOutstandingReads in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 68 == 68, "The order of defintion for 68 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLogFileCreateAsynch == 69, "The order of defintion for JET_paramLogFileCreateAsynch in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramErrorToString == 70, "The order of defintion for JET_paramErrorToString in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramZeroDatabaseDuringBackup == 71, "The order of defintion for JET_paramZeroDatabaseDuringBackup in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramUnicodeIndexDefault == 72, "The order of defintion for JET_paramUnicodeIndexDefault in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRuntimeCallback == 73, "The order of defintion for JET_paramRuntimeCallback in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_EnableReattachRaceBugFix == 74, "The order of defintion for JET_paramFlight_EnableReattachRaceBugFix in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 75 == 75, "The order of defintion for 75 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableSortedRetrieveColumns == 76, "The order of defintion for JET_paramEnableSortedRetrieveColumns in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCleanupMismatchedLogFiles == 77, "The order of defintion for JET_paramCleanupMismatchedLogFiles in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRecordUpgradeDirtyLevel == 78, "The order of defintion for JET_paramRecordUpgradeDirtyLevel in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRecoveryCurrentLogfile == 79, "The order of defintion for JET_paramRecoveryCurrentLogfile in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 80 == 80, "The order of defintion for 80 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramGlobalMinVerPages == 81, "The order of defintion for JET_paramGlobalMinVerPages in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramOSSnapshotTimeout == 82, "The order of defintion for JET_paramOSSnapshotTimeout in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 83 == 83, "The order of defintion for 83 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_NewQueueOptions == 84, "The order of defintion for JET_paramFlight_NewQueueOptions in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_ConcurrentMetedOps == 85, "The order of defintion for JET_paramFlight_ConcurrentMetedOps in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_LowMetedOpsThreshold == 86, "The order of defintion for JET_paramFlight_LowMetedOpsThreshold in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_MetedOpStarvedThreshold == 87, "The order of defintion for JET_paramFlight_MetedOpStarvedThreshold in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 88 == 88, "The order of defintion for 88 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_EnableShrinkArchiving == 89, "The order of defintion for JET_paramFlight_EnableShrinkArchiving in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_EnableBackupDuringRecovery == 90, "The order of defintion for JET_paramFlight_EnableBackupDuringRecovery in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSRollIntervalSec == 91, "The order of defintion for JET_paramFlight_RBSRollIntervalSec in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSMaxRequiredRange == 92, "The order of defintion for JET_paramFlight_RBSMaxRequiredRange in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSCleanupEnabled == 93, "The order of defintion for JET_paramFlight_RBSCleanupEnabled in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSLowDiskSpaceThresholdGb == 94, "The order of defintion for JET_paramFlight_RBSLowDiskSpaceThresholdGb in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSMaxSpaceWhenLowDiskSpaceGb == 95, "The order of defintion for JET_paramFlight_RBSMaxSpaceWhenLowDiskSpaceGb in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSMaxTimeSpanSec == 96, "The order of defintion for JET_paramFlight_RBSMaxTimeSpanSec in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramFlight_RBSCleanupIntervalMinSec == 97, "The order of defintion for JET_paramFlight_RBSCleanupIntervalMinSec in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramExceptionAction == 98, "The order of defintion for JET_paramExceptionAction in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEventLogCache == 99, "The order of defintion for JET_paramEventLogCache in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCreatePathIfNotExist == 100, "The order of defintion for JET_paramCreatePathIfNotExist in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPageHintCacheSize == 101, "The order of defintion for JET_paramPageHintCacheSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramOneDatabasePerSession == 102, "The order of defintion for JET_paramOneDatabasePerSession in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxDatabasesPerInstance == 103, "The order of defintion for JET_paramMaxDatabasesPerInstance in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxInstances == 104, "The order of defintion for JET_paramMaxInstances in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramVersionStoreTaskQueueMax == 105, "The order of defintion for JET_paramVersionStoreTaskQueueMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIdleFlushTime == 106, "The order of defintion for JET_paramIdleFlushTime in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDisablePerfmon == 107, "The order of defintion for JET_paramDisablePerfmon in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramAbortRetryFailCallback == 108, "The order of defintion for JET_paramAbortRetryFailCallback in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramVAReserve == 109, "The order of defintion for JET_paramVAReserve in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIndexTuplesLengthMin == 110, "The order of defintion for JET_paramIndexTuplesLengthMin in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIndexTuplesLengthMax == 111, "The order of defintion for JET_paramIndexTuplesLengthMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIndexTuplesToIndexMax == 112, "The order of defintion for JET_paramIndexTuplesToIndexMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramAlternateDatabaseRecoveryPath == 113, "The order of defintion for JET_paramAlternateDatabaseRecoveryPath in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 114 == 114, "The order of defintion for 114 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 115 == 115, "The order of defintion for 115 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 116 == 116, "The order of defintion for 116 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 117 == 117, "The order of defintion for 117 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 118 == 118, "The order of defintion for 118 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 119 == 119, "The order of defintion for 119 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDBAPageAvailMin == 120, "The order of defintion for JET_paramDBAPageAvailMin in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDBAPageAvailThreshold == 121, "The order of defintion for JET_paramDBAPageAvailThreshold in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDBAK1 == 122, "The order of defintion for JET_paramDBAK1 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDBAK2 == 123, "The order of defintion for JET_paramDBAK2 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxRandomIOSize == 124, "The order of defintion for JET_paramMaxRandomIOSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCachedClosedTables == 125, "The order of defintion for JET_paramCachedClosedTables in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableFileCache == 126, "The order of defintion for JET_paramEnableFileCache in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableViewCache == 127, "The order of defintion for JET_paramEnableViewCache in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramVerPageSize == 128, "The order of defintion for JET_paramVerPageSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramConfiguration == 129, "The order of defintion for JET_paramConfiguration in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableAdvanced == 130, "The order of defintion for JET_paramEnableAdvanced in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxColtyp == 131, "The order of defintion for JET_paramMaxColtyp in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIndexTupleIncrement == 132, "The order of defintion for JET_paramIndexTupleIncrement in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIndexTupleStart == 133, "The order of defintion for JET_paramIndexTupleStart in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramKeyMost == 134, "The order of defintion for JET_paramKeyMost in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCheckpointIOMax == 135, "The order of defintion for JET_paramCheckpointIOMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLegacyFileNames == 136, "The order of defintion for JET_paramLegacyFileNames in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass1Name == 137, "The order of defintion for JET_paramTableClass1Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass2Name == 138, "The order of defintion for JET_paramTableClass2Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass3Name == 139, "The order of defintion for JET_paramTableClass3Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass4Name == 140, "The order of defintion for JET_paramTableClass4Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass5Name == 141, "The order of defintion for JET_paramTableClass5Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass6Name == 142, "The order of defintion for JET_paramTableClass6Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass7Name == 143, "The order of defintion for JET_paramTableClass7Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass8Name == 144, "The order of defintion for JET_paramTableClass8Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass9Name == 145, "The order of defintion for JET_paramTableClass9Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass10Name == 146, "The order of defintion for JET_paramTableClass10Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass11Name == 147, "The order of defintion for JET_paramTableClass11Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass12Name == 148, "The order of defintion for JET_paramTableClass12Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass13Name == 149, "The order of defintion for JET_paramTableClass13Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass14Name == 150, "The order of defintion for JET_paramTableClass14Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass15Name == 151, "The order of defintion for JET_paramTableClass15Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIOPriority == 152, "The order of defintion for JET_paramIOPriority in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramWaypointLatency == 153, "The order of defintion for JET_paramWaypointLatency in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCheckpointTooDeep == 154, "The order of defintion for JET_paramCheckpointTooDeep in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDisableBlockVerification == 155, "The order of defintion for JET_paramDisableBlockVerification in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnablePersistedCallbacks == 156, "The order of defintion for JET_paramEnablePersistedCallbacks in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramAggressiveLogRollover == 157, "The order of defintion for JET_paramAggressiveLogRollover in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPeriodicLogRolloverLLR == 158, "The order of defintion for JET_paramPeriodicLogRolloverLLR in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramUsePageDependencies == 159, "The order of defintion for JET_paramUsePageDependencies in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDefragmentSequentialBTrees == 160, "The order of defintion for JET_paramDefragmentSequentialBTrees in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDefragmentSequentialBTreesDensityCheckFrequency == 161, "The order of defintion for JET_paramDefragmentSequentialBTreesDensityCheckFrequency in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramIOThrottlingTimeQuanta == 162, "The order of defintion for JET_paramIOThrottlingTimeQuanta in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramLVChunkSizeMost == 163, "The order of defintion for JET_paramLVChunkSizeMost in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxCoalesceReadSize == 164, "The order of defintion for JET_paramMaxCoalesceReadSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxCoalesceWriteSize == 165, "The order of defintion for JET_paramMaxCoalesceWriteSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxCoalesceReadGapSize == 166, "The order of defintion for JET_paramMaxCoalesceReadGapSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxCoalesceWriteGapSize == 167, "The order of defintion for JET_paramMaxCoalesceWriteGapSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableHaPublish == 168, "The order of defintion for JET_paramEnableHaPublish in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableDBScanInRecovery == 169, "The order of defintion for JET_paramEnableDBScanInRecovery in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDbScanThrottle == 170, "The order of defintion for JET_paramDbScanThrottle in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDbScanIntervalMinSec == 171, "The order of defintion for JET_paramDbScanIntervalMinSec in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDbScanIntervalMaxSec == 172, "The order of defintion for JET_paramDbScanIntervalMaxSec in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEmitLogDataCallback == 173, "The order of defintion for JET_paramEmitLogDataCallback in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEmitLogDataCallbackCtx == 174, "The order of defintion for JET_paramEmitLogDataCallbackCtx in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableExternalAutoHealing == 175, "The order of defintion for JET_paramEnableExternalAutoHealing in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPatchRequestTimeout == 176, "The order of defintion for JET_paramPatchRequestTimeout in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramCachePriority == 177, "The order of defintion for JET_paramCachePriority in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxTransactionSize == 178, "The order of defintion for JET_paramMaxTransactionSize in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPrereadIOMax == 179, "The order of defintion for JET_paramPrereadIOMax in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableDBScanSerialization == 180, "The order of defintion for JET_paramEnableDBScanSerialization in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramHungIOThreshold == 181, "The order of defintion for JET_paramHungIOThreshold in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramHungIOActions == 182, "The order of defintion for JET_paramHungIOActions in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMinDataForXpress == 183, "The order of defintion for JET_paramMinDataForXpress in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableShrinkDatabase == 184, "The order of defintion for JET_paramEnableShrinkDatabase in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( 185 == 185, "The order of defintion for 185 in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramProcessFriendlyName == 186, "The order of defintion for JET_paramProcessFriendlyName in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDurableCommitCallback == 187, "The order of defintion for JET_paramDurableCommitCallback in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableSqm == 188, "The order of defintion for JET_paramEnableSqm in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramConfigStoreSpec == 189, "The order of defintion for JET_paramConfigStoreSpec in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramStageFlighting == 190, "The order of defintion for JET_paramStageFlighting in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramZeroDatabaseUnusedSpace == 191, "The order of defintion for JET_paramZeroDatabaseUnusedSpace in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramDisableVerifications == 192, "The order of defintion for JET_paramDisableVerifications in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramPersistedLostFlushDetection == 193, "The order of defintion for JET_paramPersistedLostFlushDetection in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEngineFormatVersion == 194, "The order of defintion for JET_paramEngineFormatVersion in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramReplayThrottlingLevel == 195, "The order of defintion for JET_paramReplayThrottlingLevel in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass16Name == 196, "The order of defintion for JET_paramTableClass16Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass17Name == 197, "The order of defintion for JET_paramTableClass17Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass18Name == 198, "The order of defintion for JET_paramTableClass18Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass19Name == 199, "The order of defintion for JET_paramTableClass19Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass20Name == 200, "The order of defintion for JET_paramTableClass20Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass21Name == 201, "The order of defintion for JET_paramTableClass21Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass22Name == 202, "The order of defintion for JET_paramTableClass22Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass23Name == 203, "The order of defintion for JET_paramTableClass23Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass24Name == 204, "The order of defintion for JET_paramTableClass24Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass25Name == 205, "The order of defintion for JET_paramTableClass25Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass26Name == 206, "The order of defintion for JET_paramTableClass26Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass27Name == 207, "The order of defintion for JET_paramTableClass27Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass28Name == 208, "The order of defintion for JET_paramTableClass28Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass29Name == 209, "The order of defintion for JET_paramTableClass29Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass30Name == 210, "The order of defintion for JET_paramTableClass30Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramTableClass31Name == 211, "The order of defintion for JET_paramTableClass31Name in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramBlockCacheConfiguration == 212, "The order of defintion for JET_paramBlockCacheConfiguration in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRecordSizeMost == 213, "The order of defintion for JET_paramRecordSizeMost in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramUseFlushForWriteDurability == 214, "The order of defintion for JET_paramUseFlushForWriteDurability in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramEnableRBS == 215, "The order of defintion for JET_paramEnableRBS in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramRBSFilePath == 216, "The order of defintion for JET_paramRBSFilePath in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );
static_assert( JET_paramMaxValueInvalid == 217, "The order of defintion for JET_paramMaxValueInvalid in sysparam.xml must follow the numerical ordering of its value (as defined in jethdr.w)." );