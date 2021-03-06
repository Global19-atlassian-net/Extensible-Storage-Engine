// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "std.hxx"

#ifndef ENABLE_JET_UNIT_TEST
#error This file should only be compiled with the unit tests!
#endif

#include "PageSizeClean.hxx"

class TestCPageValidationAction : public IPageValidationAction
{
public:
    TestCPageValidationAction() { Reset(); }
    ~TestCPageValidationAction() {}

    void Reset()
    {
        m_pgno = pgnoNull;
        m_pgnoExpected = pgnoNull;
        m_err = JET_errSuccess;
        m_ibitCorrupted = 0;
        m_action = None;

        PAGECHECKSUM pagechecksumNull;
        m_checksumExpected = pagechecksumNull;
        m_checksumActual = pagechecksumNull;
    }
    
    void BadChecksum(
        const PGNO pgno,
        const ERR err,
        const PAGECHECKSUM checksumExpected,
        const PAGECHECKSUM checksumActual )
    {
        Reset();
        m_action = BadChecksumAction;
        m_pgno = pgno;
        m_err = err;
        m_checksumExpected = checksumExpected;
        m_checksumActual = checksumActual;
    }
    
    void UninitPage( const PGNO pgno, const ERR err )
    {
        Reset();
        m_action = UninitPageAction;
        m_pgno = pgno;
        m_err = err;
    }
    
    void BadPgno( const PGNO pgno, const PGNO pgnoActual, const ERR err )
    {
        Reset();
        m_action = BadPgnoAction;
        m_pgno = pgnoActual;
        m_pgnoExpected = pgno;
    }
    
    void BitCorrection( const PGNO pgno, const INT ibitCorrupted )
    {
        Reset();
    }
    
    void BitCorrectionFailed( const PGNO pgno, const ERR err, const INT ibitCorrupted )
    {
        Reset();
    }

    void LostFlush(
        const PGNO pgno,
        const FMPGNO fmpgno,
        const INT pgftExpected,
        const INT pgftActual,
        const ERR err,
        const BOOL fRuntime,
        const BOOL fFailOnRuntimeOnly )
    {
        Reset();
    }

    PGNO m_pgno;
    PGNO m_pgnoExpected;
    ERR m_err;
    PGNO m_ibitCorrupted;
    PAGECHECKSUM m_checksumExpected;
    PAGECHECKSUM m_checksumActual;

    enum Action { None, UninitPageAction, BadPgnoAction, BadChecksumAction, BitCorrectionAction, BitCorrectionFailedAction };
    Action m_action;
};

JETUNITTEST ( CPAGE, PgftGetNextFlushType )
{
    CHECK( CPAGE::pgftRockWrite == CPAGE::PgftGetNextFlushType( CPAGE::pgftUnknown ) );
    CHECK( CPAGE::pgftPaperWrite == CPAGE::PgftGetNextFlushType( CPAGE::pgftRockWrite ) );
    CHECK( CPAGE::pgftScissorsWrite == CPAGE::PgftGetNextFlushType( CPAGE::pgftPaperWrite ) );
    CHECK( CPAGE::pgftRockWrite == CPAGE::PgftGetNextFlushType( CPAGE::pgftScissorsWrite ) );
}

JETUNITTEST ( CPAGE, PageValidationLostFlushCheck )
{
#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingWithLostFlush );
#endif

    CPageValidationNullAction nullaction;

    CPAGE cpage;
    cpage.LoadNewTestPage( 4096 );
    const PGNO pgno = cpage.PgnoThis();

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );

    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, NULL ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, NULL ) );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, NULL ) );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    cpage.PreparePageForWrite( CPAGE::pgftUnknown );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, NULL ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftUnknown );
    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &nullaction, NULL ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif
}

JETUNITTEST ( CPAGE, PageValidationLostFlushCheckInconsistentFlushMapDbTime )
{
#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingWithLostFlush );
#endif

    CPageValidationNullAction nullaction;

    CPAGE cpage;
    cpage.LoadNewTestPage( 4096 );
    const PGNO pgno = cpage.PgnoThis();

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );

    cpage.SetDbtime( 100 );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite, cpage.Dbtime() );
    flushmap.SetPgnoFlushType( pgno + 1, CPAGE::pgftPaperWrite, cpage.Dbtime() );
    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    cpage.PreparePageForWrite( CPAGE::pgftScissorsWrite );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    cpage.SetDbtime( 101 );
    cpage.PreparePageForWrite( CPAGE::pgftScissorsWrite );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );
    CHECK( CPAGE::pgftPaperWrite == flushmap.PgftGetPgnoFlushType( pgno + 1 ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    CHECK( CPAGE::pgftUnknown == flushmap.PgftGetPgnoFlushType( pgno ) );
    CHECK( CPAGE::pgftUnknown == flushmap.PgftGetPgnoFlushType( pgno + 1 ) );

#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif
}

JETUNITTEST ( CPAGE, PageValidationLostFlushCheckRuntimeDetection )
{
#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingWithLostFlush );
#endif

    IFileSystemAPI* pfsapi = NULL;
    const WCHAR* const wszFmFilePath = L".\\flushmap.jfm";
    SIGNATURE signDbHdrFlushFromDb, signFlushMapHdrFlushFromDb;
    SIGGetSignature( &signDbHdrFlushFromDb );
    SIGGetSignature( &signFlushMapHdrFlushFromDb );
    CHECK( JET_errSuccess == ErrOSFSCreate( &pfsapi ) );

    CPageValidationNullAction nullaction;

    CPAGE cpage;
    cpage.LoadNewTestPage( 4096 );
    const PGNO pgno = cpage.PgnoThis();

    (void)pfsapi->ErrFileDelete( wszFmFilePath );

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetFmFilePath( wszFmFilePath );
    flushmap.SetPersisted( fTrue );
    flushmap.SetReadOnly( fFalse );
    flushmap.SetDbState( JET_dbstateDirtyShutdown );
    flushmap.SetDbGenMinRequired( 1 );
    flushmap.SetDbGenMinConsistent( 1 );
    flushmap.SetDbHdrFlushSignaturePointerFromDb( &signDbHdrFlushFromDb );
    flushmap.SetFmHdrFlushSignaturePointerFromDb( &signFlushMapHdrFlushFromDb );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == flushmap.ErrCleanFlushMap() );
    flushmap.TermFlushMap();
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfFailOnRuntimeLostFlushOnly, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfFailOnRuntimeLostFlushOnly, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfFailOnRuntimeLostFlushOnly, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfFailOnRuntimeLostFlushOnly, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftPaperWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    flushmap.TermFlushMap();

    CHECK( JET_errSuccess == pfsapi->ErrFileDelete( wszFmFilePath ) );

    delete pfsapi;

#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif
}

JETUNITTEST ( CPAGE, PageValidationLostFlushCheckRuntimeOnly )
{
#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingWithLostFlush );
#endif

    IFileSystemAPI* pfsapi = NULL;
    const WCHAR* const wszFmFilePath = L".\\flushmap.jfm";
    SIGNATURE signDbHdrFlushFromDb, signFlushMapHdrFlushFromDb;
    SIGGetSignature( &signDbHdrFlushFromDb );
    SIGGetSignature( &signFlushMapHdrFlushFromDb );
    CHECK( JET_errSuccess == ErrOSFSCreate( &pfsapi ) );

    CPageValidationNullAction nullaction;

    CPAGE cpage;
    cpage.LoadNewTestPage( 4096 );
    const PGNO pgno = cpage.PgnoThis();

    (void)pfsapi->ErrFileDelete( wszFmFilePath );

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetFmFilePath( wszFmFilePath );
    flushmap.SetPersisted( fTrue );
    flushmap.SetReadOnly( fFalse );
    flushmap.SetDbState( JET_dbstateDirtyShutdown );
    flushmap.SetDbGenMinRequired( 1 );
    flushmap.SetDbGenMinConsistent( 1 );
    flushmap.SetDbHdrFlushSignaturePointerFromDb( &signDbHdrFlushFromDb );
    flushmap.SetFmHdrFlushSignaturePointerFromDb( &signFlushMapHdrFlushFromDb );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == flushmap.ErrCleanFlushMap() );
    flushmap.TermFlushMap();
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlushIfNotRuntime, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlushIfNotRuntime, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errReadLostFlushVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlushIfNotRuntime, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftRockWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlushIfNotRuntime, &nullaction, &flushmap ) );
    CHECK( CPAGE::pgftPaperWrite == flushmap.PgftGetPgnoFlushType( pgno ) );

    flushmap.TermFlushMap();

    CHECK( JET_errSuccess == pfsapi->ErrFileDelete( wszFmFilePath ) );

    delete pfsapi;

#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif
}

JETUNITTEST ( CPAGE, PageValidationSmall )
{
    const INT cbPage = 8 * 1024;
    BYTE * const pvBuffer = (BYTE*)PvOSMemoryPageAlloc( cbPage, NULL );
    CHECK( NULL != pvBuffer );
    if( NULL == pvBuffer )
    {
        return;
    }

#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingPagePgnos );
    FNegTestSet( fPreviouslySet );
#endif

    const IFMP ifmp = 1;
    const PGNO pgno = 2;

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );
    
    CPAGE cpage;
    cpage.LoadPage( ifmp, pgno, pvBuffer, cbPage );
    CHECK( cpage.FLoadedPage() );

    TestCPageValidationAction validationaction;

    memset( pvBuffer, 0, cbPage );
    CHECK( !cpage.FPageIsInitialized() );
    CHECK( JET_errPageNotInitialized == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::UninitPageAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errPageNotInitialized == validationaction.m_err );
    
    memset( (BYTE*)pvBuffer, 0xEC, 1);
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer, 0, 1 );
    
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR), 0xEC, 1);
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR), 0, 1);
    
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR2), 0xEC, 1);
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR2), 0, 1 );
    
    memset( (BYTE*)pvBuffer+cbPage-1, 0xEC, 1 );
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer+cbPage-1, 0, 1 );
    
    cpage.UnloadPage();
    cpage.LoadNewPage( ifmp, pgno, 3, 0x0, pvBuffer, cbPage );
    memset( (BYTE*)pvBuffer, 0xEC, 16  );
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR2), 0xEC, cbPage-sizeof(CPAGE::PGHDR2)-8  );
    CHECK( cpage.FPageIsInitialized() );
    CHECK( cpage.FLoadedPage() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );

    SetPageChecksum( (BYTE *)pvBuffer, cbPage, databasePage, pgno );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );

    pvBuffer[200] = ~pvBuffer[200];
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction, &flushmap ) );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftScissorsWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction, &flushmap ) );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftScissorsWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.PreparePageForWrite( CPAGE::pgftScissorsWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftScissorsWrite );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction, &flushmap ) );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.UnloadPage();
    OSMemoryPageFree(pvBuffer);
}

JETUNITTEST ( CPAGE, PageValidationBig )
{
    const INT cbPage = 32 * 1024;

    BYTE * const pvBuffer = (BYTE*)PvOSMemoryPageAlloc( cbPage, NULL );
    CHECK( NULL != pvBuffer );
    if( NULL == pvBuffer )
    {
        return;
    }

    const IFMP ifmp = 1;
    const PGNO pgno = 2;

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno * 2 ) );

    CPAGE cpage;
    cpage.LoadPage( ifmp, pgno, pvBuffer, cbPage );
    CHECK( cpage.FLoadedPage() );

    TestCPageValidationAction validationaction;

    memset( pvBuffer, 0, cbPage );
    CHECK( !cpage.FPageIsInitialized() );
    CHECK( JET_errPageNotInitialized == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::UninitPageAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errPageNotInitialized == validationaction.m_err );
    
    memset( (BYTE*)pvBuffer, 0xEC, 1);
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer, 0, 1 );
    
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR), 0xEC, 1);
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR), 0, 1);
    
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR2), 0xEC, 1);
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR2), 0, 1 );
    
    memset( (BYTE*)pvBuffer+cbPage-1, 0xEC, 1 );
    CHECK( cpage.FPageIsInitialized() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );
    memset( (BYTE*)pvBuffer+cbPage-1, 0, 1 );

    cpage.UnloadPage();
    cpage.LoadNewPage( ifmp, pgno, 3, 0x0, pvBuffer, cbPage );
    memset( (BYTE*)pvBuffer, 0xEC, 16  );
    memset( (BYTE*)pvBuffer+sizeof(CPAGE::PGHDR2), 0xEC, cbPage-sizeof(CPAGE::PGHDR2)-8  );
    CHECK( cpage.FPageIsInitialized() );
    CHECK( cpage.FLoadedPage() );
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadChecksumAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errReadVerifyFailure == validationaction.m_err );

    cpage.SetPgno( pgno );
    SetPageChecksum( (BYTE *)pvBuffer, cbPage, databasePage, pgno );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( cpage.PgnoThis() != 0 );
    Assert( 2 == pgno );
    CHECK( cpage.PgnoThis() == pgno );

    cpage.SetPgno( pgno * 2 );
    SetPageChecksum( (BYTE *)pvBuffer, cbPage, databasePage, pgno );
    CHECK( JET_errReadPgnoVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::BadPgnoAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgnoExpected );
    CHECK( ( 2 * pgno ) == validationaction.m_pgno );

#ifdef DEBUG
    const bool fPreviouslySet = FNegTestSet( fCorruptingPagePgnos );
#endif
    CHECK( cpage.PgnoThis() == ( 2 * pgno ) );
    CHECK( cpage.PgnoThis() != pgno );
#ifdef DEBUG
    if ( !fPreviouslySet )
    {
        FNegTestUnset( fCorruptingPagePgnos );
    }
#endif

    cpage.SetPgno( pgno );
    pvBuffer[200] = ~pvBuffer[200];
#ifdef DEBUG
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    CHECK( JET_errReadVerifyFailure == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
#ifdef DEBUG
    FNegTestSet( fPreviouslySet );
#endif

    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction, &flushmap ) );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftScissorsWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.PreparePageForWrite( CPAGE::pgftPaperWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction, &flushmap ) );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftScissorsWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.PreparePageForWrite( CPAGE::pgftScissorsWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftScissorsWrite );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction, &flushmap ) );
    CHECK( JET_errSuccess                       == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fCorruptingWithLostFlush );
#endif
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftPaperWrite );
    CHECK( JET_errReadLostFlushVerifyFailure    == cpage.ErrValidatePage( pgvfDefault, &validationaction, &flushmap ) );
#ifndef RTM
    FNegTestSet( fPreviouslySet );
#endif

    cpage.UnloadPage();
    OSMemoryPageFree(pvBuffer);
}

JETUNITTEST ( CPAGE, ShrunkPageValidation4KB )
{
    const INT cbPage = 4 * 1024;
    BYTE * const pvBuffer = (BYTE*)PvOSMemoryPageAlloc( cbPage, NULL );
    CHECK( NULL != pvBuffer );
    if( NULL == pvBuffer )
    {
        return;
    }

#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingPagePgnos );
    FNegTestSet( fPreviouslySet );
#endif

    const IFMP ifmp = 1;
    const PGNO pgno = 2;
    
    TestCPageValidationAction validationaction;

    CPAGE cpage;
    cpage.GetShrunkPage( ifmp, pgno, pvBuffer, cbPage );
    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();

    CHECK( cpage.FLoadedPage() );
    CHECK( cpage.FShrunkPage() );
    CHECK( pgno == cpage.PgnoThis() );
    CHECK( ifmp == cpage.Ifmp() );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( pgchk == cpage.LoggedDataChecksum() );

    memset( pvBuffer, 0, cbPage );
    CHECK( !cpage.FPageIsInitialized() );
    CHECK( JET_errPageNotInitialized == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::UninitPageAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errPageNotInitialized == validationaction.m_err );

    cpage.UnloadPage();

    OSMemoryPageFree(pvBuffer);
}

JETUNITTEST ( CPAGE, ShrunkPageValidation32KB )
{
    const INT cbPage = 32 * 1024;
    BYTE * const pvBuffer = (BYTE*)PvOSMemoryPageAlloc( cbPage, NULL );
    CHECK( NULL != pvBuffer );
    if( NULL == pvBuffer )
    {
        return;
    }

#ifndef RTM
    const bool fPreviouslySet = FNegTestSet( fCorruptingPagePgnos );
    FNegTestSet( fPreviouslySet );
#endif

    const IFMP ifmp = 1;
    const PGNO pgno = 2;
    
    TestCPageValidationAction validationaction;

    CPAGE cpage;
    cpage.GetShrunkPage( ifmp, pgno, pvBuffer, cbPage );
    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();

    CHECK( cpage.FLoadedPage() );
    CHECK( cpage.FShrunkPage() );
    CHECK( pgno == cpage.PgnoThis() );
    CHECK( ifmp == cpage.Ifmp() );
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( pgchk == cpage.LoggedDataChecksum() );

    memset( pvBuffer, 0, cbPage );
    CHECK( !cpage.FPageIsInitialized() );
    CHECK( JET_errPageNotInitialized == cpage.ErrValidatePage( pgvfDoNotCheckForLostFlush, &validationaction ) );
    CHECK( TestCPageValidationAction::UninitPageAction == validationaction.m_action );
    CHECK( pgno == validationaction.m_pgno );
    CHECK( JET_errPageNotInitialized == validationaction.m_err );

    cpage.UnloadPage();

    OSMemoryPageFree(pvBuffer);
}

JETUNITTESTEX( CPAGE, ConcurrentMultiPageSizedUsage, JetSimpleUnitTest::dwBufferManager )
{

    const ULONG cbSmallPage = 4 * 1024;
    const ULONG cbLargePage = 32 * 1024;

    CPAGE cpageSmall;
    CPAGE cpageLarge;

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );

    cpageSmall.LoadNewTestPage( cbSmallPage );
    cpageLarge.LoadNewTestPage( cbLargePage );

    CHECK( cpageSmall.PgnoThis() == cpageLarge.PgnoThis() );
    const PGNO pgno = cpageSmall.PgnoThis();

    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );

#ifdef DEBUG
    CHECK( cpageSmall.CbPage() == cbSmallPage );
    CHECK( cpageLarge.CbPage() == cbLargePage );
#endif


    BYTE rgbData[1000];
    memset( rgbData, 0x3c, sizeof(rgbData) );
    USHORT * pus = (USHORT*)rgbData;
    pus[0] = 0x0004;

    for( INT i = 0; i < 4; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        cpageSmall.Insert( i%3, &data, 1, 0 );
        cpageLarge.Insert( i%3, &data, 1, 0 );
    }


    CHECK( cpageSmall.CbPageFree() < 100 );
    CHECK( cpageLarge.CbPageFree() > 27 * 1024 );


    for( INT i = 0; i < 8; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        cpageLarge.Insert( 2+i, &data, 1, 0 );
    }


    cpageSmall.Delete( 1 );
    cpageLarge.Delete( 2 );
    cpageLarge.Delete( 10 );

    CHECK( cpageSmall.CbPageFree() > 1004  );

    CHECK( JET_errSuccess == cpageSmall.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( JET_errSuccess == cpageLarge.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );


    for( INT i = 0; i < 22; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        cpageLarge.Insert( 2+i, &data, 1, 0 );
    }
    CHECK( cpageLarge.CbPageFree() < 1024 );

    DATA data;
    data.SetPv( rgbData );
    data.SetCb( sizeof(rgbData) );
    cpageSmall.Insert( 1, &data, 1, 0 );

    CHECK( cpageSmall.CbPageFree() < 100 );


    cpageSmall.PreparePageForWrite( CPAGE::pgftRockWrite );
    cpageLarge.PreparePageForWrite( CPAGE::pgftRockWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );

    CPageValidationNullAction nullaction;

    CHECK( JET_errSuccess == cpageSmall.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpageLarge.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );
    CHECK( JET_errSuccess == cpageSmall.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( JET_errSuccess == cpageLarge.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

}


BOOL memchk( __in const void * pv, __in char ch, __in const ULONG cb )
{
    const char * pb = (const char *)pv;
    ULONG ib = 0;
    for( ib = 0; ib < cb; ib++ )
    {
        if ( pb[ib] != ch )
        {
            return fFalse;
        }
    }
    return fTrue;
}

JETUNITTESTEX( CPAGE, PageHydrationBasic32KB, JetSimpleUnitTest::dwBufferManager )
{
    ULONG cbPage = 32 * 1024;
    ULONG cbBuffer = cbPage;
    ULONG cbNextSize;

    CPAGE cpage;
    cpage.LoadNewTestPage( cbPage );

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    const PGNO pgno = cpage.PgnoThis();
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );

#ifdef DEBUG
    CHECK( cpage.CbPage() == cbPage );
#endif
    CHECK( cpage.FIsNormalSized() );


    BYTE rgbData[1000];

    for( INT i = 0; i < 12; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        if ( i <= 9 )
        {
            memset( rgbData, '0' + i, sizeof(rgbData) );
        }
        else
        {
            memset( rgbData, 'a' + i - 10, sizeof(rgbData) );
        }
        USHORT * pus = (USHORT*)rgbData;
        pus[0] = 0x0003;

        cpage.Insert( i, &data, 1, 0 );
    }

    CHECK( cpage.CbPageFree() > 20 * 1024 );


    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );

    cbBuffer = roundup( cbNextSize, OSMemoryPageCommitGranularity() );

#ifdef _IA64_
    CHECK( cbBuffer == 16 * 1024 );
#else
    CHECK( cbBuffer == 12 * 1024 );
#endif

    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();


    cpage.DehydratePage( cbBuffer, fFalse );

#ifdef DEBUG
#ifdef _IA64_
    CHECK( cpage.CbBuffer() == 16 * 1024 );
#else
    CHECK( cpage.CbBuffer() == 12 * 1024 );
#endif
#endif
    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( !cpage.FIsNormalSized() );
    CHECK( cpage.CbPageFree() > 20 * 1024 );
    CHECK( pgchk == cpage.LoggedDataChecksum() );


    LINE line;

    cpage.GetPtrExternalHeader( &line );
    CHECK( line.cb == 0 );
    
    cpage.GetPtr( 1, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '1', line.cb-2 ) );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );

    cpage.GetPtr( 10, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, 'a', line.cb-2 ) );

    cpage.FRootPage();


    cpage.ReplaceFlags( 4, 0x1);


    CHECK( !cpage.FPageIsDehydratable( &cbNextSize, fFalse  ) );
    CHECK( cbNextSize >= 12 * 1024 );
    const PAGECHECKSUM pgchk2 = cpage.LoggedDataChecksum();


    cpage.RehydratePage();

    CHECK( cpage.CbPageFree() > 20 * 1024 );
#ifdef DEBUG
    CHECK( cpage.CbBuffer() == cbPage );
#endif
    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk2 == cpage.LoggedDataChecksum() );


    cpage.Delete( 8 );
    cpage.Delete( 6 );
    cpage.Delete( 5 );
    cpage.Delete( 4 );
    cpage.Delete( 4 );


    const void * pv1; size_t cb1; const void * pv2; size_t cb2;
    cpage.ReorganizePage( &pv1, &cb1, &pv2, &cb2 );

    CHECK( cpage.CbPageFree() > 25 * 1024 );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );


    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( cbNextSize <= 8 * 1024 );
    cbBuffer = 8 * 1024;
    const PAGECHECKSUM pgchk3 = cpage.LoggedDataChecksum();

    cpage.DehydratePage( cbBuffer, fFalse );

#ifdef DEBUG
    CHECK( cpage.CbBuffer() == 8 * 1024 );
#endif

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk3 == cpage.LoggedDataChecksum() );

    const PAGECHECKSUM pgchk4 = cpage.LoggedDataChecksum();


    cpage.RehydratePage();

    CHECK( cpage.CbPageFree() > 25 * 1024 );
    CHECK( cpage.FIsNormalSized() );

    CHECK( pgchk4 == cpage.LoggedDataChecksum() );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );

    for( INT i = 0; i < 25; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        cpage.Insert( 4+i, &data, 1, 0 );
    }
    CHECK( cpage.CbPageFree() < 1024 );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );

    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );


    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CPageValidationNullAction nullaction;
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

}

JETUNITTESTEX( CPAGE, EmptyPageDehydration16KB, JetSimpleUnitTest::dwBufferManager )
{
    ULONG cbPage = 16 * 1024;
    ULONG cbBuffer = cbPage;
    ULONG cbNextSize;

    CPAGE cpage;
    cpage.LoadNewTestPage( cbPage );
    cpage.SetDbtime( 0x7f7adaf323 );

#ifdef DEBUG
    CHECK( cpage.CbPage() == cbPage );
#endif
    CHECK( cpage.FIsNormalSized() );


    CHECK( cpage.CbPageFree() > 12 * 1024 );

    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );

    cbBuffer = roundup( cbNextSize, OSMemoryPageCommitGranularity() );

    CHECK( cbBuffer == OSMemoryPageCommitGranularity() );

    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();


    cpage.DehydratePage( cbBuffer, fFalse );

#ifdef DEBUG
    CHECK( cpage.CbBuffer() == OSMemoryPageCommitGranularity() );
#endif
    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( !cpage.FIsNormalSized() );
    CHECK( pgchk == cpage.LoggedDataChecksum() );


    cpage.RehydratePage();

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( cpage.FIsNormalSized() );
    CHECK( pgchk == cpage.LoggedDataChecksum() );


    BYTE rgbData[1000];

    for( INT i = 0; i < 5; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        memset( rgbData, '0' + i, sizeof(rgbData) );
        cpage.Insert( i, &data, 1, 0 );
    }

}

JETUNITTESTEX( CPAGE, PageHydrationBasic8KB, JetSimpleUnitTest::dwBufferManager )
{
    ULONG cbPage = 8 * 1024;
    ULONG cbBuffer = cbPage;
    ULONG cbNextSize;

    CPAGE cpage;
    cpage.LoadNewTestPage( cbPage );

#ifdef DEBUG
    CHECK( cpage.CbPage() == cbPage );
#endif
    CHECK( cpage.FIsNormalSized() );


    BYTE rgbData[1000];

    for( INT i = 0; i < 8; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        memset( rgbData, '0' + i, sizeof(rgbData) );
        cpage.Insert( i, &data, 1, 0 );
    }

    CHECK( cpage.CbPageFree() < 200 );


    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );

    cbBuffer = roundup( cbNextSize, OSMemoryPageCommitGranularity() );

    CHECK( cbBuffer == 8 * 1024 );


    cpage.Delete( 3 );
    cpage.Delete( 3 );
    cpage.Delete( 3 );
    cpage.Delete( 3 );

    const void * pv1; size_t cb1; const void * pv2; size_t cb2;
    cpage.ReorganizePage( &pv1, &cb1, &pv2, &cb2 );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

#ifdef _IA64_
    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );

    return;
#else
    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( cbNextSize <= 4 * 1024 );
    cbBuffer = 4 * 1024;
#endif

    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();


    cpage.DehydratePage( cbBuffer, fFalse );

#ifdef DEBUG
    CHECK( cpage.CbBuffer() == 4 * 1024 );
#endif

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk == cpage.LoggedDataChecksum() );


    LINE line;

    cpage.GetPtrExternalHeader( &line );
    CHECK( line.cb == 0 );
    
    cpage.GetPtr( 1, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '1', line.cb-2 ) );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '7' , line.cb-2 ) );

    cpage.FRootPage();


    cpage.RehydratePage();

    CHECK( cpage.CbPageFree() > 4 * 1024 );

#ifdef DEBUG
    CHECK( cpage.CbBuffer() == cbPage );
#endif

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk == cpage.LoggedDataChecksum() );

    cpage.GetPtr( 1, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '1', line.cb-2 ) );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '7' , line.cb-2 ) );

}

JETUNITTESTEX( CPAGE, PageHydrationBasic4KB, JetSimpleUnitTest::dwBufferManager )
{
    ULONG cbPage = 4 * 1024;
    ULONG cbBuffer = cbPage;
    ULONG cbNextSize;

    CPAGE cpage;
    cpage.LoadNewTestPage( cbPage );

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    const PGNO pgno = cpage.PgnoThis();
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );

#ifdef DEBUG
    CHECK( cpage.CbPage() == cbPage );
#endif
    CHECK( cpage.FIsNormalSized() );


    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );


    BYTE rgbData[1000];

    for( INT i = 0; i < 4; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        memset( rgbData, '0' + i, sizeof(rgbData) );
        cpage.Insert( i, &data, 1, 0 );
    }

    CHECK( cpage.CbPageFree() < 100 );


    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );

    cpage.Delete( 1 );
    cpage.Delete( 1 );
    cpage.Delete( 1 );

    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();

    const void * pv1; size_t cb1; const void * pv2; size_t cb2;
    cpage.ReorganizePage( &pv1, &cb1, &pv2, &cb2 );

    CHECK( pgchk == cpage.LoggedDataChecksum() );

    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );

    cpage.Delete( 0 );
    cpage.ReorganizePage( &pv1, &cb1, &pv2, &cb2 );

    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );



    FNegTestSet( fInvalidUsage );
    cpage.DehydratePage( cbBuffer, fFalse );
    FNegTestUnset( fInvalidUsage );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );


    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CPageValidationNullAction nullaction;
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

}

JETUNITTESTEX( CPAGE, DirtyPageDehydration, JetSimpleUnitTest::dwBufferManager )
{
    ULONG cbPage = 32 * 1024;

    CPAGE cpage;
    cpage.LoadNewTestPage( cbPage );

#ifdef DEBUG
    CHECK( cpage.CbPage() == cbPage );
#endif
    CHECK( cpage.FIsNormalSized() );


    BYTE rgbData[1000];


    for( INT i = 0; i < 12; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        if ( i <= 9 )
        {
            memset( rgbData, '0' + i, sizeof(rgbData) );
        }
        else
        {
            memset( rgbData, 'a' + i - 10, sizeof(rgbData) );
        }
        cpage.Insert( i, &data, 1, 0 );
    }

    CHECK( cpage.CbPageFree() > 20 * 1024 );

    ULONG cbNextSize;


    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );
    cbNextSize = roundup( cbNextSize, OSMemoryPageCommitGranularity() );

    cpage.DehydratePage( cbNextSize, fFalse );

#ifdef DEBUG
#ifdef _IA64_
    CHECK( cpage.CbBuffer() == 16 * 1024 );
#else
    CHECK( cpage.CbBuffer() == 12 * 1024 );
#endif
#endif
    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( !cpage.FIsNormalSized() );

    return;

}

#ifdef FDTC_0_REORG_DEHYDRATE

JETUNITTESTEX( CPAGE, PageHydrationReorganizationBasic32KB, JetSimpleUnitTest::dwBufferManager )
{
    ULONG cbPage = 32 * 1024;
    ULONG cbBuffer = cbPage;
    ULONG cbNextSize;

    CPAGE cpage;
    cpage.LoadNewTestPage( cbPage );

    CFlushMapForUnattachedDb flushmap;
    flushmap.SetPersisted( fFalse );
    CHECK( JET_errSuccess == flushmap.ErrInitFlushMap() );
    const PGNO pgno = cpage.PgnoThis();
    CHECK( JET_errSuccess == flushmap.ErrSetFlushMapCapacity( pgno ) );

    OnDebug( CHECK( cpage.CbPage() == cbPage ) );
    CHECK( cpage.FIsNormalSized() );


    BYTE rgbData[1000];

    for( INT i = 0; i < 12; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        if ( i <= 9 )
        {
            memset( rgbData, '0' + i, sizeof(rgbData) );
        }
        else
        {
            memset( rgbData, 'a' + i - 10, sizeof(rgbData) );
        }
        cpage.Insert( i, &data, 1, 0 );
    }

    CHECK( cpage.CbPageFree() > 20 * 1024 );


    CHECK( cpage.FPageIsDehydratable( &cbNextSize ) );
    cbBuffer = roundup( cbNextSize, 4096 );
    CHECK( cbBuffer == 12 * 1024 );
    const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();


    cpage.DehydratePage( cbBuffer, fTrue );

    OnDebug( CHECK( cpage.CbBuffer() == 12 * 1024 ) );
    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( !cpage.FIsNormalSized() );
    CHECK( cpage.CbPageFree() > 20 * 1024 );
    CHECK( pgchk == cpage.LoggedDataChecksum() );


    LINE line;

    cpage.GetPtrExternalHeader( &line );
    CHECK( line.cb == 0 );
    
    cpage.GetPtr( 1, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '1', line.cb-2 ) );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );

    cpage.GetPtr( 10, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, 'a', line.cb-2 ) );

    cpage.FRootPage();


    cpage.ReplaceFlags( 4, 0x1);


    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );
    CHECK( cbNextSize >= 11 * 1024 );
    const PAGECHECKSUM pgchk2 = cpage.LoggedDataChecksum();


    cpage.RehydratePage();

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk2 == cpage.LoggedDataChecksum() );


    cpage.Delete( 8 );
    cpage.Delete( 6 );
    cpage.Delete( 5 );
    cpage.Delete( 4 );
    cpage.Delete( 4 );

    CHECK( 12 * 1024 == cbBuffer );
    cpage.DehydratePage( cbBuffer, fFalse );

    ULONG cbOldSize = cbBuffer;

    CHECK( !cpage.FPageIsDehydratable( &cbNextSize, fFalse ) );

    CHECK( cpage.FPageIsDehydratable( &cbNextSize, fTrue ) );


    CHECK( cpage.CbPageFree() > 25 * 1024 );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );


    const PAGECHECKSUM pgchk25 = cpage.LoggedDataChecksum();

    FNegTestSet( fInvalidUsage );
    cpage.DehydratePage( cbOldSize, fTrue );
    FNegTestUnset( fInvalidUsage );

    OnDebug( CHECK( cpage.CbBuffer() == 12 * 1024 ) );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk25 == cpage.LoggedDataChecksum() );


    const PAGECHECKSUM pgchk3 = cpage.LoggedDataChecksum();


    CHECK( cpage.FPageIsDehydratable( &cbNextSize, fTrue ) );

    cbBuffer = roundup( cbNextSize, 4096 );
    CHECK( cbBuffer == 8 * 1024 );
    


    CHECK( cbNextSize >= 4 * 1024 );
    CHECK( cbNextSize <= 8 * 1024 );

    CHECK( cbNextSize <= 8 * 1024 );
    cbBuffer = 8 * 1024;

    cpage.DehydratePage( cbBuffer, fTrue );

    OnDebug( CHECK( cpage.CbBuffer() == 8 * 1024 ) );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
    CHECK( pgchk3 == cpage.LoggedDataChecksum() );

    const PAGECHECKSUM pgchk4 = cpage.LoggedDataChecksum();

    cpage.RehydratePage();

    CHECK( cpage.CbPageFree() > 25 * 1024 );
    CHECK( cpage.FIsNormalSized() );

    CHECK( pgchk4 == cpage.LoggedDataChecksum() );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );

    for( INT i = 0; i < 25; ++i )
    {
        DATA data;
        data.SetPv( rgbData );
        data.SetCb( sizeof(rgbData) );
        cpage.Insert( 4+i, &data, 1, 0 );
    }
    CHECK( cpage.CbPageFree() < 1024 );

    cpage.GetPtr( 3, &line );
    CHECK( sizeof(rgbData) == line.cb );
    CHECK( memchk( (BYTE*)line.pv+2, '3', line.cb-2 ) );

    CHECK( !cpage.FPageIsDehydratable( &cbNextSize ) );


    cpage.PreparePageForWrite( CPAGE::pgftRockWrite );
    flushmap.SetPgnoFlushType( pgno, CPAGE::pgftRockWrite );
    CPageValidationNullAction nullaction;
    CHECK( JET_errSuccess == cpage.ErrValidatePage( pgvfDefault, &nullaction, &flushmap ) );

    CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );

}

JETUNITTESTEX( CPAGE, PageHydrationReorganizationBoundary32KB, JetSimpleUnitTest::dwBufferManager )
{
    const ULONG cbPage = 32 * 1024;
    ULONG cbBuffer = cbPage;
    ULONG cbNextSize;

    BYTE * pbBuffer = NULL;
    BFAlloc( bfasTemporary, (void**)&pbBuffer, cbPage );


    for ( ULONG cbLast = 3; cbLast <= 5; cbLast++ )
    {
        for ( INT i = 3; i <= 32 * 1024; i++ )
        {
            cbBuffer = cbPage;
            CPAGE cpage;
            cpage.LoadNewTestPage( cbPage );
            
            OnDebug( CHECK( cpage.CbPage() == cbPage ) );
            CHECK( cpage.FIsNormalSized() );
            
            
            const INT ilineFirst = 0;
            const INT ilineToDelete = 1;
            INT ilineLast = 2;

            DATA data;

            if ( cpage.CbPageFree() < 1000 + 4  )
            {
                break;
            }

            const ULONG cbFirst = 1000;
            data.SetPv( pbBuffer );
            data.SetCb( cbFirst );
            memset( pbBuffer+1, 'F', cbFirst-1 );
            cpage.Insert( ilineFirst, &data, 1, 0 );

            if ( cpage.CbPageFree() < i + 4  )
            {
                break;
            }

            const ULONG cbToDelete = i;
            data.SetPv( pbBuffer );
            data.SetCb( cbToDelete );
            memset( pbBuffer+1, 'D', cbToDelete-1 );
            cpage.Insert( ilineToDelete, &data, 1, 0 );

            if ( cpage.CbPageFree() < cbLast + 4  )
            {
                break;
            }

            data.SetPv( pbBuffer );
            data.SetCb( cbLast );
            memset( pbBuffer+1, 'L', cbLast-1 );
            cpage.Insert( ilineLast, &data, 1, 0 );

            LINE lineToDelete;
            cpage.GetPtr( ilineToDelete, &lineToDelete );
            LINE lineLast;
            cpage.GetPtr( ilineLast, &lineLast );
            CHECK( lineToDelete.pv < lineLast.pv );

            CHECK( ((CHAR*)lineLast.pv)[2] == 'L' );
            CHECK( cbLast == 3 || ((CHAR*)lineLast.pv)[3] == 'L' );

            BOOL fDehydratedWithoutReorg = fFalse;

            if ( cpage.FPageIsDehydratable( &cbNextSize, fFalse ) )
            {
                cbBuffer = roundup( cbNextSize, 4096 );
                const PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();
                cpage.DehydratePage( cbBuffer, fFalse );
                CHECK( !cpage.FIsNormalSized() );
                CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
                CHECK( pgchk == cpage.LoggedDataChecksum() );
                fDehydratedWithoutReorg = fTrue;
            }
            CHECK( !cpage.FPageIsDehydratable( &cbNextSize, fFalse ) );
            CHECK( !cpage.FPageIsDehydratable( &cbNextSize, fTrue ) );

            cpage.GetPtr( ilineLast, &lineLast );
            CHECK( lineLast.cb == cbLast );
            CHECK( ((CHAR*)lineLast.pv)[2] == 'L' );
            CHECK( cbLast == 3 || ((CHAR*)lineLast.pv)[3] == 'L' );

            if ( fDehydratedWithoutReorg )
            {
                cpage.RehydratePage();
                CHECK( cpage.FPageIsDehydratable( &cbNextSize, fFalse ) );
                CHECK( cbBuffer >= cbNextSize );
            }

            CHECK( cpage.FIsNormalSized() );

            CHECK( cpage.FPageIsDehydratable( &cbNextSize, fFalse )
                    || (ULONG)i >= ( 27573 - ( cbLast - 3 ) ) );

            const PAGECHECKSUM pgchkPreDelete = cpage.LoggedDataChecksum();

            cpage.GetPtr( ilineLast, &lineLast );
            void * pvLast = lineLast.pv;

            cpage.Delete( ilineToDelete );
            ilineLast--;

            cpage.GetPtr( ilineLast, &lineLast );
            CHECK( pvLast == lineLast.pv );

            PAGECHECKSUM pgchk = cpage.LoggedDataChecksum();
            CHECK( pgchkPreDelete != pgchk );

            if ( fDehydratedWithoutReorg )
            {

                CHECK( cpage.FPageIsDehydratable( &cbNextSize, fFalse ) );
                CHECK( cbBuffer >= cbNextSize );
                cpage.DehydratePage( cbBuffer, fFalse );

                CHECK( !cpage.FIsNormalSized() );
                CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
                CHECK( pgchk == cpage.LoggedDataChecksum() );

                cpage.GetPtr( ilineLast, &lineLast );
                CHECK( pvLast == lineLast.pv );

                CHECK( !cpage.FPageIsDehydratable( &cbNextSize, fFalse ) ||
                        ( ( cbNextSize % 4096 ) >= 4093 ) );
            }

            if ( i > 4096 )
            {
                CHECK( cpage.FPageIsDehydratable( &cbNextSize, fTrue ) );
                cbBuffer = roundup( cbNextSize, 4096 );
                OnDebug( CHECK( cbBuffer < cpage.CbBuffer() ) );

                cpage.DehydratePage( cbBuffer, fTrue );

                CHECK( !cpage.FIsNormalSized() );
                OnDebug( CHECK( cbBuffer == cpage.CbBuffer() ) );
                CHECK( JET_errSuccess == cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) );
                CHECK( pgchk == cpage.LoggedDataChecksum() );

                OnDebug( CHECK( 4096 == cpage.CbBuffer() ) );

                cpage.GetPtr( ilineLast, &lineLast );
                CHECK( pvLast != lineLast.pv );

                CHECK( !cpage.FPageIsDehydratable( &cbNextSize, fTrue ) );
            }

            cpage.GetPtr( ilineLast, &lineLast );
            CHECK( lineLast.cb == cbLast );
            CHECK( ((CHAR*)lineLast.pv)[2] == 'L' );
            CHECK( cbLast == 3 || ((CHAR*)lineLast.pv)[3] == 'L' );

#ifdef DEBUG
            i += rand() % 100;
#endif
        }
    }

    BFFree( pbBuffer );
}

#endif

JETUNITTESTEX ( CPAGE, ErrCheckPagesPerf, JetSimpleUnitTest::dwDontRunByDefault )
{
    const INT cbPage = 32 * 1024;
    BYTE * const pvBuffer = (BYTE*)PvOSMemoryPageAlloc( cbPage, NULL );
    CHECK( NULL != pvBuffer );
    if ( NULL == pvBuffer )
    {
        return;
    }

    printf( "\n" );
    
    BYTE rgbData[ 25 ];
    INT cIterations = 50000;

    DATETIME dtm;
    UtilGetCurrentDateTime( &dtm );
    Assert( dtm.year != 0 && dtm.month != 0 && dtm.day != 0 );
    const INT seed = dtm.year * 8772  + dtm.month * 731  + dtm.day * 24 + dtm.hour;
    srand( seed );
    printf( "Seeding based upon hour (of year): %d   (i.e. variably switches every hour)\n", seed );

    for ( INT cRecords = 100; cRecords <= 1000; cRecords *= 10 )
    {
        CPAGE cpage;
        cpage.LoadNewTestPage( cbPage );
        CHECK( cpage.FLoadedPage() );

        for ( INT i = 0; i < cRecords; ++i )
        {
            DATA data;
            data.SetPv( rgbData );

            INT cbRecord = 2 + ( rand() % ( sizeof(rgbData) - 1 ) );
            data.SetCb( cbRecord );
            Assert( cbRecord >= 2 );
            
            if ( i <= 9 )
            {
                memset( rgbData, '0' + i, sizeof(rgbData) );
            }
            else
            {
                memset( rgbData, 'a' + i - 10, sizeof(rgbData) );
            }
            *(UnalignedLittleEndian<USHORT>*)rgbData = (USHORT)min( ( cbRecord - 2 ) / 2, 10 );
            
            cpage.Insert( i, &data, 1, 0 );
        }
        
    {
        DWORD dwStart = TickOSTimeCurrent();
        
        for ( INT i = 0; i < cIterations; i++ )
        {

            CHECK( cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckBasics ) == JET_errSuccess );
        }
        DWORD dwEnd = TickOSTimeCurrent();
        printf( "CheckBasics: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
                cRecords,
                cIterations,
                (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
    }

    {
        DWORD dwStart = TickOSTimeCurrent();

        for ( INT i = 0; i < cIterations; i++ )
        {

            CHECK( cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckTagsNonOverlapping ) == JET_errSuccess );
        }
        DWORD dwEnd = TickOSTimeCurrent();
        printf( "CheckTagsNonOverlapping: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
                cRecords,
                cIterations,
                (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
    }
    
    {
        DWORD dwStart = TickOSTimeCurrent();

        for ( INT i = 0; i < cIterations; i++ )
        {

            CHECK( cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckLineBoundedByTag ) == JET_errSuccess );
        }
        DWORD dwEnd = TickOSTimeCurrent();
        printf( "CheckLineBoundedByTag: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
                cRecords,
                cIterations,
                (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
    }

    {
        DWORD dwStart = TickOSTimeCurrent();

        for ( INT i = 0; i < cIterations; i++ )
        {

            CHECK( cpage.ErrCheckPage( CPRINTFDBGOUT::PcprintfInstance(), CPAGE::OnErrorReturnError, CPAGE::CheckPageExtensiveness( CPAGE::CheckTagsNonOverlapping | CPAGE::CheckLineBoundedByTag ) ) == JET_errSuccess );
        }
        DWORD dwEnd = TickOSTimeCurrent();
        printf( "NonOverlapping+BoundedByTag: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
                cRecords,
                cIterations,
                (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
    }
    
        cpage.UnloadPage();
    }
    
    OSMemoryPageFree(pvBuffer);
}

JETUNITTESTEX ( CPAGE, VersionCheckFlagsPerf, JetSimpleUnitTest::dwDontRunByDefault )
{
    const INT cbPage = 32 * 1024;
    printf( "\n" );
    
    BYTE rgbData[ 25 ];
    INT cIterations = 50000;
    
    for ( INT cRecords = 100; cRecords <= 1000; cRecords *= 10 )
    {
        CPAGE cpage;
        cpage.LoadNewTestPage( cbPage );
        CHECK( cpage.FLoadedPage() );

        for ( INT i = 0; i < cRecords; ++i )
        {
            DATA data;
            data.SetPv( rgbData );
            INT cbRecord = 5;
            data.SetCb( cbRecord );
            
            cpage.Insert( i, &data, 1, 0 );
        }

        DWORD dwStart = TickOSTimeCurrent();
        
        for ( INT i = 0; i < cIterations; i++ )
        {

            cpage.FAnyLineHasFlagSet( fNDVersion );
        }

        DWORD dwEnd = TickOSTimeCurrent();
        printf( "CheckAnyLineHasFlags: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
                cRecords,
                cIterations,
                (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
        
        cpage.UnloadPage();
    }
    
}

JETUNITTESTEX ( CPAGE, VersionClearFlagsPerf, JetSimpleUnitTest::dwDontRunByDefault )
{
    const INT cbPage = 32 * 1024;
    printf( "\n" );
    
    BYTE rgbData[ 25 ];
    INT cIterations = 50000;
    
    for ( INT cRecords = 100; cRecords <= 1000; cRecords *= 10 )
    {
        CPAGE cpage;
        cpage.LoadNewTestPage( cbPage );
        CHECK( cpage.FLoadedPage() );

        for ( INT i = 0; i < cRecords; ++i )
        {
            DATA data;
            data.SetPv( rgbData );
            INT cbRecord = 5;
            data.SetCb( cbRecord );
            
            cpage.Insert( i, &data, 1, fNDVersion );
        }

        DWORD dwStart = TickOSTimeCurrent();
        
        for ( INT i = 0; i < cIterations; i++ )
        {

            cpage.ResetAllFlags( fNDVersion );
        }
        
        DWORD dwEnd = TickOSTimeCurrent();
        printf( "ResetFlags: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
                cRecords,
                cIterations,
                (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
        
        cpage.UnloadPage();
    }
    
}


static bool FAreLinesEqual( const void* pv0, const void* pv1, ULONG cb)
{
    Assert( cbKeyCount <= cb );

    const USHORT cbKeyFlags0 = ( USHORT )*( UnalignedLittleEndian< USHORT >* )pv0;
    const USHORT cbKeyFlags1 = ( USHORT )*( UnalignedLittleEndian< USHORT >* )pv1;
    const USHORT cbKey0 = cbKeyFlags0 & 0x1fff;
    const USHORT cbKey1 = cbKeyFlags1 & 0x1fff;

    Assert( sizeof( cbKey0 ) == cbKeyCount );
    Assert( sizeof( cbKey1 ) == cbKeyCount );

    const BYTE* const pb0 = ( const BYTE* )pv0;
    const BYTE* const pb1 = ( const BYTE* )pv1;

    return cbKey0 == cbKey1 && !memcmp( pb0 + cbKeyCount, pb1 + cbKeyCount, cb - cbKeyCount );
}

class CPageTestFixture : public JetTestFixture
{
    private:
        CPAGE   m_cpage;
        void *  m_pvPage;
        void *  m_pvBuffer;

        static const IFMP   m_ifmp      = 1;
        static const PGNO   m_pgno      = 2;
        static const OBJID  m_objidFDP  = 3;
        static const ULONG  m_fFlags    = CPAGE::fPageRoot | CPAGE::fPageLeaf;

    protected:
        CPageTestFixture();
        CPageTestFixture( const BOOL fRuntimeScrubbingEnabled );
        ~CPageTestFixture();
        void InitMembers_()
        {
            m_pvBuffer = NULL;
        }
        
        virtual INT CbPage_() = 0;

        bool SetUp_();
        void TearDown_();

    private:
        void InsertOneLine_( const ULONG cb );

        void InsertString_( INT iline, const char * const sz );
        void ReplaceString_( INT iline, const char * const sz );
        INT CInstancesOnPage_( const char * const sz );
        
    public:
        void TestMaxNodeSize()
        {
            const INT cbMaxNodeOld = 4047 + 1;
            CHECK( CPAGE::CbPageDataMax( CbPage_() ) >= cbMaxNodeOld || CbPage_() < 4 * 1024 );
        }
        
        void TestLoadNewPage();
        void TestInternalTest();

        void TestSetPgnoNext()
        {
            m_cpage.SetPgnoNext(100);
            CHECK(100 == m_cpage.PgnoNext());
        }
        void TestSetPgnoPrev()
        {
            m_cpage.SetPgnoPrev(101);
            CHECK(101 == m_cpage.PgnoPrev());
        }
        void TestSetDbtime()
        {
            m_cpage.SetDbtime( 0x123456 );
            CHECK( 0x123456 == m_cpage.Dbtime() );
        }
        void TestSetFlags()
        {
            m_cpage.SetFlags( CPAGE::fPageLeaf | CPAGE::fPageLongValue );
            CHECK( ( CPAGE::fPageLeaf | CPAGE::fPageLongValue ) == m_cpage.FFlags() );
        }
        void TestSetFEmpty()
        {
            m_cpage.SetFEmpty();
            CHECK( m_cpage.FEmptyPage() );
        }
        void TestRevertDbtimeCheckDbtime();
        void TestRevertDbtimeCheckFlags();
        void TestFlags();
        void TestLoggedDataChecksum();
        void TestInsert();
        void TestInsertToCapacity();
        void TestReplace();
        void TestLastDeleteFixesContiguousSpace();
        void TestDeleteScrubsData();
        void TestReplaceScrubsData();
        void TestReplaceScrubsGapData();
        void TestReplaceScrubsFirstGap();
        void TestReorganizePageScrubsData();
        void TestInsertLines();
        void TestReorganizePage();
        void TestOverwriteUnusedSpace();
        void TestOverwriteUnusedSpaceFixesContiguousSpaceForNonZeroRecords();
        void TestReplacePerf();
        
};

template <INT PAGESIZE_IN_KB, BOOL RUNTIME_SCRUBBING_ENABLED>
class CPageTestFixturePageSize : public CPageTestFixture
{
public:
    CPageTestFixturePageSize() : CPageTestFixture( RUNTIME_SCRUBBING_ENABLED ) {}

    ~CPageTestFixturePageSize() {}

protected:
    INT CbPage_() { return PAGESIZE_IN_KB * 1024; }
};

#define CPAGE_TEST(func) \
    static const JetTestCaller<CPageTestFixturePageSize<4, fFalse>> test4kScrubOff##func("CPAGE." #func "4KBScrubOff", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<4, fFalse>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<8, fFalse>> test8kScrubOff##func("CPAGE." #func "8KBScrubOff", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<8, fFalse>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<16, fFalse>> test16kScrubOff##func("CPAGE." #func "16KBScrubOff", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<16, fFalse>::Test##func);\
    static const JetTestCaller<CPageTestFixturePageSize<32, fFalse>> test32kScrubOff##func("CPAGE." #func "32KBScrubOff", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<32, fFalse>::Test##func);\
    static const JetTestCaller<CPageTestFixturePageSize<4, fTrue>> test4kScrubOn##func("CPAGE." #func "4KBScrubOn", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<4, fTrue>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<8, fTrue>> test8kScrubOn##func("CPAGE." #func "8KBScrubOn", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<8, fTrue>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<16, fTrue>> test16kScrubOn##func("CPAGE." #func "16KBScrubOn", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<16, fTrue>::Test##func);\
    static const JetTestCaller<CPageTestFixturePageSize<32, fTrue>> test32kScrubOn##func("CPAGE." #func "32KBScrubOn", JetSimpleUnitTest::dwBufferManager, &CPageTestFixturePageSize<32, fTrue>::Test##func);\

#define CPAGE_TEST_EX(func, flags) \
    static const JetTestCaller<CPageTestFixturePageSize<4, fFalse>> test4kScrubOff##func("CPAGE." #func "4KBScrubOff", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<4, fFalse>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<8, fFalse>> test8kScrubOff##func("CPAGE." #func "8KBScrubOff", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<8, fFalse>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<16, fFalse>> test16kScrubOff##func("CPAGE." #func "16KBScrubOff", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<16, fFalse>::Test##func);\
    static const JetTestCaller<CPageTestFixturePageSize<32, fFalse>> test32kScrubOff##func("CPAGE." #func "32KBScrubOff", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<32, fFalse>::Test##func);\
    static const JetTestCaller<CPageTestFixturePageSize<4, fTrue>> test4kScrubOn##func("CPAGE." #func "4KBScrubOn", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<4, fTrue>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<8, fTrue>> test8kScrubOn##func("CPAGE." #func "8KBScrubOn", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<8, fTrue>::Test##func);   \
    static const JetTestCaller<CPageTestFixturePageSize<16, fTrue>> test16kScrubOn##func("CPAGE." #func "16KBScrubOn", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<16, fTrue>::Test##func);\
    static const JetTestCaller<CPageTestFixturePageSize<32, fTrue>> test32kScrubOn##func("CPAGE." #func "32KBScrubOn", JetSimpleUnitTest::dwBufferManager | ( flags ), &CPageTestFixturePageSize<32, fTrue>::Test##func);\

CPAGE_TEST(MaxNodeSize);
CPAGE_TEST(LoadNewPage);
CPAGE_TEST(SetPgnoNext);
CPAGE_TEST(SetPgnoPrev);
CPAGE_TEST(SetDbtime);
CPAGE_TEST(RevertDbtimeCheckDbtime);
CPAGE_TEST(RevertDbtimeCheckFlags);
CPAGE_TEST(SetFlags);
CPAGE_TEST(SetFEmpty);
CPAGE_TEST(Flags);
CPAGE_TEST(LoggedDataChecksum);
CPAGE_TEST(Insert);
CPAGE_TEST(InsertToCapacity);
CPAGE_TEST(Replace);
CPAGE_TEST(LastDeleteFixesContiguousSpace);
CPAGE_TEST(DeleteScrubsData);
CPAGE_TEST(ReplaceScrubsData);
CPAGE_TEST(ReplaceScrubsGapData);
CPAGE_TEST(ReplaceScrubsFirstGap);
CPAGE_TEST(ReorganizePageScrubsData);
CPAGE_TEST(InsertLines);
CPAGE_TEST(ReorganizePage);
CPAGE_TEST(OverwriteUnusedSpace);
CPAGE_TEST(OverwriteUnusedSpaceFixesContiguousSpaceForNonZeroRecords);
CPAGE_TEST_EX(ReplacePerf, JetSimpleUnitTest::dwDontRunByDefault);
CPAGE_TEST(InternalTest);

void CPageTestFixture::TestLoadNewPage()
{
    CHECK( m_ifmp == m_cpage.Ifmp() );
    CHECK( m_objidFDP == m_cpage.ObjidFDP() );
    CHECK( m_fFlags == (m_cpage.FFlags() & m_fFlags) );
}

void CPageTestFixture::TestInternalTest()
{
    const ERR err = m_cpage.ErrTest( CbPage_() );
    CHECK( JET_errSuccess == err );
}

void CPageTestFixture::TestRevertDbtimeCheckDbtime()
{
    m_cpage.SetDbtime( 1 );
    CHECK( 1 == m_cpage.Dbtime() );
    m_cpage.SetDbtime( 2 );
    CHECK( 2 == m_cpage.Dbtime() );
    m_cpage.RevertDbtime( 1, CPAGE::fPageRoot );
    CHECK( 1 == m_cpage.Dbtime() );
}

void CPageTestFixture::TestRevertDbtimeCheckFlags()
{
    ULONG fFlagsBefore = 0;
    ULONG fFlagsAfter = 0;

    fFlagsBefore = CPAGE::fPageLeaf | CPAGE::fPageLongValue;
    m_cpage.SetFlags( fFlagsBefore );
    CHECK( !m_cpage.FScrubbed() );
    CHECK( fFlagsBefore == m_cpage.FFlags() );
    fFlagsAfter = CPAGE::fPageLeaf | CPAGE::fPageLongValue | CPAGE::fPageRoot;
    m_cpage.SetFlags( fFlagsAfter );
    CHECK( !m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == m_cpage.FFlags() );
    m_cpage.SetDbtime( 2 );
    CHECK( 2 == m_cpage.Dbtime() );
    m_cpage.RevertDbtime( 1, fFlagsBefore );
    CHECK( 1 == m_cpage.Dbtime() );
    CHECK( !m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == m_cpage.FFlags() );

    fFlagsBefore = CPAGE::fPageLeaf | CPAGE::fPageLongValue;
    m_cpage.SetFlags( fFlagsBefore );
    CHECK( !m_cpage.FScrubbed() );
    CHECK( fFlagsBefore == m_cpage.FFlags() );
    fFlagsAfter = CPAGE::fPageLeaf | CPAGE::fPageLongValue | CPAGE::fPageRoot | CPAGE::fPageScrubbed;
    m_cpage.SetFlags( fFlagsAfter );
    CHECK( m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == m_cpage.FFlags() );
    m_cpage.SetDbtime( 2 );
    CHECK( 2 == m_cpage.Dbtime() );
    m_cpage.RevertDbtime( 1, fFlagsBefore );
    CHECK( 1 == m_cpage.Dbtime() );
    CHECK( !m_cpage.FScrubbed() );
    CHECK( ( fFlagsAfter & ~CPAGE::fPageScrubbed ) == m_cpage.FFlags() );

    fFlagsBefore = CPAGE::fPageLeaf | CPAGE::fPageLongValue | CPAGE::fPageScrubbed;
    m_cpage.SetFlags( fFlagsBefore );
    CHECK( m_cpage.FScrubbed() );
    CHECK( fFlagsBefore == m_cpage.FFlags() );
    fFlagsAfter = CPAGE::fPageLeaf | CPAGE::fPageLongValue | CPAGE::fPageRoot | CPAGE::fPageScrubbed;
    m_cpage.SetFlags( fFlagsAfter );
    CHECK( m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == m_cpage.FFlags() );
    m_cpage.SetDbtime( 2 );
    CHECK( 2 == m_cpage.Dbtime() );
    m_cpage.RevertDbtime( 1, fFlagsBefore );
    CHECK( 1 == m_cpage.Dbtime() );
    CHECK( m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == m_cpage.FFlags() );

    fFlagsBefore = CPAGE::fPageLeaf | CPAGE::fPageLongValue | CPAGE::fPageScrubbed;
    m_cpage.SetFlags( fFlagsBefore );
    CHECK( m_cpage.FScrubbed() );
    CHECK( fFlagsBefore == m_cpage.FFlags() );
    fFlagsAfter = CPAGE::fPageLeaf | CPAGE::fPageLongValue | CPAGE::fPageRoot;
    m_cpage.SetFlags( fFlagsAfter );
    CHECK( !m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == m_cpage.FFlags() );
    m_cpage.SetDbtime( 2 );
    CHECK( 2 == m_cpage.Dbtime() );
    m_cpage.RevertDbtime( 1, fFlagsBefore );
    CHECK( 1 == m_cpage.Dbtime() );
    CHECK( m_cpage.FScrubbed() );
    CHECK( fFlagsAfter == ( m_cpage.FFlags() & ~CPAGE::fPageScrubbed ) );
}

void CPageTestFixture::TestFlags()
{
    m_cpage.SetFlags( CPAGE::fPageRoot | CPAGE::fPageLeaf | CPAGE::fPageLongValue );
    CHECK( ( CPAGE::fPageRoot | CPAGE::fPageLeaf | CPAGE::fPageLongValue ) == m_cpage.FFlags() );
    CHECK( m_cpage.FLeafPage() );
    CHECK( !m_cpage.FInvisibleSons() );
    CHECK( m_cpage.FRootPage() );
    CHECK( m_cpage.FFDPPage() );
    CHECK( !m_cpage.FEmptyPage() );
    CHECK( !m_cpage.FPreInitPage() );
    CHECK( !m_cpage.FParentOfLeaf() );
    CHECK( !m_cpage.FSpaceTree() );
    CHECK( !m_cpage.FRepairedPage() );
    CHECK( m_cpage.FPrimaryPage() );
    CHECK( !m_cpage.FIndexPage() );
    CHECK( m_cpage.FLongValuePage() );
    CHECK( !m_cpage.FScrubbed() );
    m_cpage.SetFEmpty();
    CHECK( m_cpage.FLeafPage() );
    CHECK( !m_cpage.FInvisibleSons() );
    CHECK( m_cpage.FRootPage() );
    CHECK( m_cpage.FFDPPage() );
    CHECK( m_cpage.FEmptyPage() );
    CHECK( !m_cpage.FPreInitPage() );
    CHECK( !m_cpage.FParentOfLeaf() );
    CHECK( !m_cpage.FSpaceTree() );
    CHECK( !m_cpage.FRepairedPage() );
    CHECK( m_cpage.FPrimaryPage() );
    CHECK( !m_cpage.FIndexPage() );
    CHECK( m_cpage.FLongValuePage() );
    m_cpage.SetFlags( CPAGE::fPageRoot | CPAGE::fPageLeaf | CPAGE::fPageScrubbed );
    CHECK( m_cpage.FPrimaryPage() );
    CHECK( !m_cpage.FIndexPage() );
    CHECK( !m_cpage.FLongValuePage() );
    m_cpage.SetFlags( CPAGE::fPagePreInit );
    CHECK( m_cpage.FPreInitPage() );
    CHECK( !m_cpage.FEmptyPage() );
    CHECK( !m_cpage.FPrimaryPage() );
    CHECK( !m_cpage.FIndexPage() );
    CHECK( !m_cpage.FLongValuePage() );
    CHECK( !m_cpage.FRootPage() );
    CHECK( !m_cpage.FInvisibleSons() );
    CHECK( !m_cpage.FParentOfLeaf() );
    CHECK( !m_cpage.FLeafPage() );
    m_cpage.SetFlags( 0 );
    CHECK( 0 == m_cpage.FFlags() );
}

void CPageTestFixture::TestLoggedDataChecksum()
{
    PAGECHECKSUM    checksumLoggedData1 = 0;
    PAGECHECKSUM    checksumLoggedData2 = 0;

    CPAGE::PGHDR * const ppghdr = (CPAGE::PGHDR*)m_cpage.PvBuffer();

    checksumLoggedData1 = m_cpage.LoggedDataChecksum();

    m_cpage.SetCbUncommittedFree( 0x50 );
    CHECK( 0x50 == m_cpage.CbUncommittedFree() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
    m_cpage.AddUncommittedFreed( 0x25 );
    CHECK( 0x75 == m_cpage.CbUncommittedFree() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
    m_cpage.ReclaimUncommittedFreed( 0x10 );
    CHECK( 0x65 == m_cpage.CbUncommittedFree() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
    m_cpage.SetCbUncommittedFree( 0x0 );
    CHECK( 0x0 == m_cpage.CbUncommittedFree() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );

    m_cpage.SetFlushType_( CPAGE::pgftUnknown );
    CHECK( CPAGE::pgftUnknown == m_cpage.Pgft() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
    m_cpage.SetFlushType_( CPAGE::pgftRockWrite );
    CHECK( CPAGE::pgftRockWrite == m_cpage.Pgft() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
    m_cpage.SetFlushType_( CPAGE::pgftPaperWrite );
    CHECK( CPAGE::pgftPaperWrite == m_cpage.Pgft() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
    m_cpage.SetFlushType_( CPAGE::pgftScissorsWrite );
    CHECK( CPAGE::pgftScissorsWrite == m_cpage.Pgft() );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );

    ppghdr->fFlags |= CPAGE::fPageNewChecksumFormat;
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );
}

void CPageTestFixture::TestInsert()
{
    
    DATA rgdata[3];
    BYTE * pb = (BYTE *)m_pvBuffer;
    ULONG cbTotal = 0;
    for( INT idata = 0; idata < _countof(rgdata); ++idata )
    {
        const INT cbToInsert = 100 + (idata * 100);
        rgdata[idata].SetPv( pb+cbTotal );
        rgdata[idata].SetCb( cbToInsert );
        cbTotal += cbToInsert;
    }

    m_cpage.Insert( 0, rgdata, _countof(rgdata), fNDVersion | fNDCompressed );

    LINE line;
    m_cpage.GetPtr( 0, &line );
    CHECK( line.cb == cbTotal );
    CHECK( FAreLinesEqual( line.pv, m_pvBuffer, cbTotal ) );
    CHECK( ( fNDVersion | fNDCompressed ) == line.fFlags );

    DATA data;
    data.SetPv( (BYTE *)m_pvBuffer + 500 );
    data.SetCb( 200 );
    m_cpage.Insert( 0, &data, 1, 0 );

    m_cpage.GetPtr( 0, &line );
    CHECK( line.cb == (ULONG)data.Cb() );
    CHECK( FAreLinesEqual( line.pv, data.Pv(), data.Cb() ) );
    CHECK( 0 == line.fFlags );

    m_cpage.GetPtr( 1, &line );
    CHECK( line.cb == cbTotal );
    CHECK( FAreLinesEqual( line.pv, m_pvBuffer, cbTotal ) );
    CHECK( ( fNDVersion | fNDCompressed ) == line.fFlags );
    
}

void CPageTestFixture::TestInsertToCapacity()
{
    INT cbFreeInitial = m_cpage.CbPageFree();

    DATA data;
    data.SetPv( m_pvBuffer );
    data.SetCb( cbFreeInitial - 4 );

    INT iline = 0;
    m_cpage.Insert( iline, &data, 1, fNDVersion );

    m_cpage.SetPgno(100);
    SetPageChecksum( (BYTE *)m_pvPage, m_cpage.CbPage(), databasePage, 100 );

#ifdef DEBUG
    m_cpage.DebugCheckAll();
#endif
}

void CPageTestFixture::TestReplace()
{
    for( INT i = 0; i < 7; ++i )
    {
        DATA data;
        data.SetPv( m_pvBuffer );
        data.SetCb( i * 64 + 2 );
        m_cpage.Insert( 0, &data, 1, 0 );
    }

    DATA data;
    data.SetPv( (BYTE *)m_pvBuffer + 1 );
    data.SetCb( m_cpage.CbPageFree() - CPAGE::cbInsertionOverhead );
    m_cpage.Replace( 5, &data, 1, fNDVersion );

    LINE line;
    m_cpage.GetPtr( 5, &line );
    CHECK( line.cb == (ULONG)data.Cb() );
    CHECK( FAreLinesEqual( line.pv, data.Pv(), data.Cb() ) );
    CHECK( fNDVersion == line.fFlags );
}

void CPageTestFixture::TestLastDeleteFixesContiguousSpace()
{
    const char * const sz = "Delete me";
    const INT iline = 0;

    const INT cbContiguousInitial = m_cpage.CbContiguousBufferFree_();
    
    InsertString_( iline, sz );
    InsertString_( iline + 1, sz );

    const INT cbContiguousAfterInserts = m_cpage.CbContiguousBufferFree_();
    CHECK( cbContiguousAfterInserts < cbContiguousInitial );

    m_cpage.Delete( iline );

    m_cpage.Delete( iline );
    const INT cbContiguousAfterLastDelete = m_cpage.CbContiguousBufferFree_();
    CHECK( cbContiguousAfterLastDelete == cbContiguousInitial );
    
}

void CPageTestFixture::TestDeleteScrubsData()
{
    const char * const sz = "Delete me";
    const INT iline = 0;

    InsertString_( iline, sz );
    CHECK( 1 == CInstancesOnPage_( sz ) );

    m_cpage.Delete( iline );
    const INT cInstancesExpected = m_cpage.FRuntimeScrubbingEnabled_() ? 0 : 1;
    CHECK( cInstancesExpected == CInstancesOnPage_( sz ) );
}

void CPageTestFixture::TestReplaceScrubsData()
{
    const char * const szBeforeImage    = "FooFooFooFooFooFooFooFooFoo";
    const char * const szAfterImage     = "Bar";
    const INT iline = 0;

    InsertString_( iline, szBeforeImage );
    CHECK( 9 == CInstancesOnPage_( "Foo" ) );
    CHECK( 0 == CInstancesOnPage_( "Bar" ) );

    ReplaceString_( iline, szAfterImage );
    const INT cInstancesExpected = m_cpage.FRuntimeScrubbingEnabled_() ? 0 : 8;
    CHECK( cInstancesExpected == CInstancesOnPage_( "Foo" ) );
    CHECK( 1 == CInstancesOnPage_( "Bar" ) );
}

void CPageTestFixture::TestReplaceScrubsGapData()
{
    const char * const szBeforeImage    = "FooFooFooFooFooFooFooFooFoo";
    const char * const szAfterImage     = "Bar";
    const INT iline = 0;

    InsertString_( iline, szBeforeImage );
    InsertString_( iline + 1, "Another string" );
    CHECK( 9 == CInstancesOnPage_( "Foo" ) );
    CHECK( 0 == CInstancesOnPage_( "Bar" ) );

    ReplaceString_( iline, szAfterImage );
    const INT cInstancesExpected = m_cpage.FRuntimeScrubbingEnabled_() ? 0 : 8;
    CHECK( cInstancesExpected == CInstancesOnPage_( "Foo" ) );
    CHECK( 1 == CInstancesOnPage_( "Bar" ) );
}

void CPageTestFixture::TestReplaceScrubsFirstGap()
{
    const char * const szBeforeImage    = "Bar";
    const char * const szAfterImage     = "FooFooFooFooFooFooFooFooFoo";
    const INT iline = 0;

    InsertString_( iline, szBeforeImage );
    InsertString_( iline + 1, "Another string" );
    CHECK( 0 == CInstancesOnPage_( "Foo" ) );
    CHECK( 1 == CInstancesOnPage_( "Bar" ) );

    ReplaceString_( iline, szAfterImage );
    CHECK( 9 == CInstancesOnPage_( "Foo" ) );
    const INT cInstancesExpected = m_cpage.FRuntimeScrubbingEnabled_() ? 0 : 1;
    CHECK( cInstancesExpected == CInstancesOnPage_( "Bar" ) );
}

void CPageTestFixture::TestReorganizePageScrubsData()
{
    DATA data;
    data.SetPv( m_pvBuffer );
    data.SetCb( 50 );
    memset( m_pvBuffer, 0, 50 );
    for( INT iline = 0; iline < 20; ++iline )
    {
        m_cpage.Insert( iline, &data, 1, fNDVersion );
    }

    const char * const sz = "F00";
    InsertString_( 20, sz );
    CHECK( 1 == CInstancesOnPage_( sz ) );

    for( INT iline = 19; iline > 0; iline -= 3 )
    {
        m_cpage.Delete( iline );
    }

    const VOID * pvHeader;
    size_t cbHeader;
    const VOID * pvTrailer;
    size_t cbTrailer;

    const PAGECHECKSUM checksumLoggedDataBefore = m_cpage.LoggedDataChecksum();
    m_cpage.ReorganizePage( &pvHeader, &cbHeader, &pvTrailer, &cbTrailer );
    const PAGECHECKSUM checksumLoggedDataAfter = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedDataBefore == checksumLoggedDataAfter );
    const INT cInstancesExpected = m_cpage.FRuntimeScrubbingEnabled_() ? 1 : 2;
    CHECK( cInstancesExpected == CInstancesOnPage_( sz ) );
}

void CPageTestFixture::TestInsertLines()
{
    for( ULONG cb = 2; cb < 17; cb++ )
    {
        InsertOneLine_( cb );
    }
    InsertOneLine_( CPAGE::CbPageDataMax( CbPage_() ) / 2 );
    for( ULONG cb = CPAGE::CbPageDataMax( CbPage_() )-16; cb < CPAGE::CbPageDataMax( CbPage_() ); cb++ )
    {
        InsertOneLine_( cb );
    }
}

void CPageTestFixture::TestReorganizePage()
{
    DATA data;
    data.SetPv( m_pvBuffer );
    data.SetCb( 50 );

    for( INT iline = 0; iline < 20; ++iline )
    {
        m_cpage.Insert( iline, &data, 1, fNDVersion );
    }

    for( INT iline = 19; iline > 0; iline -= 3 )
    {
        m_cpage.Delete( iline );
    }
    
    const VOID * pvHeader;
    size_t cbHeader;
    const VOID * pvTrailer;
    size_t cbTrailer;

    INT cbContiguousFreeBefore = m_cpage.CbContiguousFree_();

    const PAGECHECKSUM checksumLoggedDataBefore = m_cpage.LoggedDataChecksum();
    m_cpage.ReorganizePage( &pvHeader, &cbHeader, &pvTrailer, &cbTrailer );
    const PAGECHECKSUM checksumLoggedDataAfter = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedDataBefore == checksumLoggedDataAfter );

    INT cbContiguousFreeAfter = m_cpage.CbContiguousFree_();
    CHECK( cbContiguousFreeAfter >= cbContiguousFreeBefore +  50 * 6 );
    
}

void CPageTestFixture::TestOverwriteUnusedSpace()
{
    INT iline = 0;
    InsertString_( iline + 0, "AAAA");
    InsertString_( iline + 1, "BBBB");
    InsertString_( iline + 2, "CCCC");

    m_cpage.Delete( iline + 1 );

    m_cpage.OverwriteUnusedSpace( chPAGETestUsedPageFreeFill );

    CHECK( 1 == CInstancesOnPage_( "AAAA" ) );
    CHECK( 0 == CInstancesOnPage_( "BBBB" ) );
    CHECK( 1 == CInstancesOnPage_( "CCCC" ) );

    CHECK( 1 < CInstancesOnPage_( "X" ) );
    
}


void CPageTestFixture::TestOverwriteUnusedSpaceFixesContiguousSpaceForNonZeroRecords()
{
    INT iline = 0;

    InsertString_( iline + 0, "AAAA");
    INT cbContiguousFreeInitial = m_cpage.CbContiguousFree_();

    InsertString_( iline + 1, "BBBB");
    InsertString_( iline + 2, "CCCC");

    m_cpage.Delete( iline + 1 );
    m_cpage.Delete( iline + 1 );

    CHECK( m_cpage.CbContiguousFree_() < cbContiguousFreeInitial );

    m_cpage.OverwriteUnusedSpace( chPAGETestUsedPageFreeFill );

    INT cbContiguousFreeAfter = m_cpage.CbContiguousFree_();
    CHECK( cbContiguousFreeAfter == cbContiguousFreeInitial );
    
}


void CPageTestFixture::TestReplacePerf()
{
    printf( "\n" );
    
#ifdef DEBUG
    INT cIterations = 50;
#else
    INT cIterations = 150000;
#endif

    INT cRecords = 0;
    while ( m_cpage.CbContiguousFree_() > 50 )
    {
        InsertString_( cRecords, "X" );
        cRecords++;
    }

    DWORD dwStart = TickOSTimeCurrent();
    
    for ( INT i = 0; i < cIterations; i++ )
    {

        ReplaceString_( i % cRecords, "test" );
        ReplaceString_( i % cRecords, "X" );
    }
    
    DWORD dwEnd = TickOSTimeCurrent();
    printf( "Replace: cRecords = %d, iterations = %d, per-iteration time (us) = %f \n",
            cRecords,
            cIterations,
            (double)(dwEnd - dwStart) / (double)cIterations * 1000.0 );
}


void CPageTestFixture::InsertString_( INT iline, const char * const sz )
{
    BYTE rgbHeader[4] = {0};

    DATA rgdata[2];
    rgdata[0].SetPv( rgbHeader );
    rgdata[0].SetCb( sizeof(rgbHeader ) );
    rgdata[1].SetPv( const_cast<char *>( sz ) );
    rgdata[1].SetCb( LOSStrLengthA(sz) );

    m_cpage.Insert( iline, rgdata, 2, 0 );
}

void CPageTestFixture::ReplaceString_( INT iline, const char * const sz )
{
    BYTE rgbHeader[4] = {0};

    DATA rgdata[2];
    rgdata[0].SetPv( rgbHeader );
    rgdata[0].SetCb( sizeof(rgbHeader ) );
    rgdata[1].SetPv( const_cast<char *>( sz ) );
    rgdata[1].SetCb( LOSStrLengthA(sz) );

    m_cpage.Replace( iline, rgdata, 2, 0 );
}

INT CPageTestFixture::CInstancesOnPage_( const char * const sz )
{

    const INT cch = LOSStrLengthA( sz );
    const char * szT = (const char *)m_pvPage;
    const char * const szMax = szT + CbPage_() - cch;

    INT cinstances = 0;
    while( szT + cch <= szMax )
    {
        INT i;
        for ( i = 0; i < cch; i++ )
        {
            if ( sz[i] != szT[i] )
            {
                break;
            }
        }
        if ( i == cch )
        {
            ++cinstances;
        }
        szT++;
    }
    return cinstances;
}

void CPageTestFixture::InsertOneLine_( const ULONG cb )
{
    PAGECHECKSUM    checksumLoggedData1 = 0;
    PAGECHECKSUM    checksumLoggedData2 = 0;
    
    DATA data;
    LINE line;
    LINE lineSav;

    data.SetPv( m_pvBuffer );
    data.SetCb( cb );

    checksumLoggedData1 = m_cpage.LoggedDataChecksum();
    m_cpage.Insert( 0, &data, 1, fNDVersion );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 != checksumLoggedData2 );

    m_cpage.GetPtr( 0, &lineSav );
    CHECK( lineSav.cb == (ULONG)cb );
    CHECK( FAreLinesEqual( lineSav.pv, m_pvBuffer, cb ) );
    CHECK( fNDVersion == lineSav.fFlags );
    
    checksumLoggedData1 = m_cpage.LoggedDataChecksum();
    m_cpage.Replace( 0, &data, 1, fNDDeleted | fNDCompressed );
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 != checksumLoggedData2 );

    m_cpage.GetPtr( 0, &line );
    CHECK( line.cb == (ULONG)cb );
    CHECK( line.pv == lineSav.pv );
    CHECK( ( fNDDeleted | fNDCompressed ) == line.fFlags );

    
    checksumLoggedData1 = m_cpage.LoggedDataChecksum();
    
    m_cpage.Replace( 0, &data, 1, fNDDeleted | fNDCompressed | fNDVersion );
    m_cpage.GetPtr( 0, &line );
    CHECK( line.cb == (ULONG)cb );
    CHECK( line.pv == lineSav.pv );
    CHECK( ( fNDDeleted | fNDCompressed | fNDVersion ) == line.fFlags );
    
    checksumLoggedData2 = m_cpage.LoggedDataChecksum();
    CHECK( checksumLoggedData1 == checksumLoggedData2 );

    m_cpage.Delete( 0 );
    CHECK( 0x0 == m_cpage.Clines() );
}

CPageTestFixture::CPageTestFixture()
{
    InitMembers_();
}

CPageTestFixture::CPageTestFixture( const BOOL fRuntimeScrubbingEnabled )
{
    InitMembers_();
    m_cpage.SetRuntimeScrubbingEnabled_( fRuntimeScrubbingEnabled );
}

CPageTestFixture::~CPageTestFixture()
{
}

bool CPageTestFixture::SetUp_()
{
    m_pvBuffer = PvOSMemoryPageAlloc( CbPage_(), NULL );
    if( NULL == m_pvBuffer )
    {
        return false;
    }
    m_pvPage = PvOSMemoryPageAlloc( CbPage_(), NULL );
    if( NULL == m_pvPage )
    {
        return false;
    }

    for( size_t idw = 0; idw < ( CbPage_() / sizeof(DWORD) ); ++idw )
    {
        ((DWORD *)m_pvBuffer)[idw] = rand();
    }

    m_cpage.LoadNewPage(
            m_ifmp,
            m_pgno,
            m_objidFDP,
            m_fFlags,
            m_pvPage,
            CbPage_() );
    
    return true;
}

void CPageTestFixture::TearDown_()
{
    m_cpage.UnloadPage();
    OSMemoryPageFree( m_pvPage );
    OSMemoryPageFree( m_pvBuffer );
    m_pvBuffer = 0;
    m_pvPage = 0;
}


