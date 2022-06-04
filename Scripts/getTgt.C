int nDigits( int t_int )
{
    int count = 0;

    if( t_int == 0 )
        return 1;

    while( t_int != 0 ) {
        t_int = t_int / 10;
        ++count;
    }

    return count;
}

void getTgt( std::string nRun ) 
{
    std::string fileName = "gntp." + nRun + ".gst.root";
    
    TFile* file = TFile::Open( fileName.c_str() );
    TTree* tree = ( TTree* )file->Get( "gst;1" );
    
    const unsigned int size = tree->GetEntries();
    int tgt_cur;

    tree->SetBranchAddress( "tgt", &tgt_cur );

    // Yes, I realize there are much better ways to do the following:
    int pdg2212       = 0;
    int pdg11         = 0;
    int pdg1000010010 = 0;
    int pdg1000060120 = 0;
    int pdg1000070140 = 0;
    int pdg1000080160 = 0;
    int pdg1000110230 = 0;
    int pdg1000120240 = 0;
    int pdg1000130270 = 0;
    int pdg1000140280 = 0;
    int pdg1000160320 = 0;
    int pdg1000180400 = 0;
    int pdg1000190390 = 0;
    int pdg1000200400 = 0;
    int pdg1000260560 = 0;
    int pdg1000641570 = 0;

    for( Int_t i = 0; i < size; i++ ) {
        tree->GetEntry( i );

             if( tgt_cur == 2212       ) ++pdg2212      ;
        else if( tgt_cur == 11         ) ++pdg11        ;
        else if( tgt_cur == 1000010010 ) ++pdg1000010010;
        else if( tgt_cur == 1000060120 ) ++pdg1000060120;
        else if( tgt_cur == 1000070140 ) ++pdg1000070140;
        else if( tgt_cur == 1000080160 ) ++pdg1000080160;
        else if( tgt_cur == 1000110230 ) ++pdg1000110230;
        else if( tgt_cur == 1000120240 ) ++pdg1000120240;
        else if( tgt_cur == 1000130270 ) ++pdg1000130270;
        else if( tgt_cur == 1000140280 ) ++pdg1000140280;
        else if( tgt_cur == 1000160320 ) ++pdg1000160320;
        else if( tgt_cur == 1000180400 ) ++pdg1000180400;
        else if( tgt_cur == 1000190390 ) ++pdg1000190390;
        else if( tgt_cur == 1000200400 ) ++pdg1000200400;
        else if( tgt_cur == 1000260560 ) ++pdg1000260560;
        else if( tgt_cur == 1000641570 ) ++pdg1000641570;
        else cout << "Unknown tgt: " << tgt_cur << endl;
    }

    int max = 0;
    if( max < pdg2212       ) max = pdg2212      ;
    if( max < pdg11         ) max = pdg11        ;
    if( max < pdg1000010010 ) max = pdg1000010010;
    if( max < pdg1000060120 ) max = pdg1000060120;
    if( max < pdg1000070140 ) max = pdg1000070140;
    if( max < pdg1000080160 ) max = pdg1000080160;
    if( max < pdg1000110230 ) max = pdg1000110230;
    if( max < pdg1000120240 ) max = pdg1000120240;
    if( max < pdg1000130270 ) max = pdg1000130270;
    if( max < pdg1000140280 ) max = pdg1000140280;
    if( max < pdg1000160320 ) max = pdg1000160320;
    if( max < pdg1000180400 ) max = pdg1000180400;
    if( max < pdg1000190390 ) max = pdg1000190390;
    if( max < pdg1000200400 ) max = pdg1000200400;
    if( max < pdg1000260560 ) max = pdg1000260560;
    if( max < pdg1000641570 ) max = pdg1000641570;

    cout << "[-]" << endl
         << " |---o code: 2212       [    p] -----> number-of-initial-interactions = " << pdg2212       << std::setw( nDigits( max ) + 2 - nDigits( pdg2212       ) ) << "(" << pdg2212       / double( size ) * 100 << "%)" << endl
         << " |---o code: 11         [   e-] -----> number-of-initial-interactions = " << pdg11         << std::setw( nDigits( max ) + 2 - nDigits( pdg11         ) ) << "(" << pdg11         / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000010010 [   H1] -----> number-of-initial-interactions = " << pdg1000010010 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000010010 ) ) << "(" << pdg1000010010 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000060120 [  C12] -----> number-of-initial-interactions = " << pdg1000060120 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000060120 ) ) << "(" << pdg1000060120 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000070140 [  N14] -----> number-of-initial-interactions = " << pdg1000070140 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000070140 ) ) << "(" << pdg1000070140 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000080160 [  O16] -----> number-of-initial-interactions = " << pdg1000080160 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000080160 ) ) << "(" << pdg1000080160 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000110230 [ Na23] -----> number-of-initial-interactions = " << pdg1000110230 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000110230 ) ) << "(" << pdg1000110230 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000120240 [ Mg24] -----> number-of-initial-interactions = " << pdg1000120240 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000120240 ) ) << "(" << pdg1000120240 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000130270 [ Al27] -----> number-of-initial-interactions = " << pdg1000130270 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000130270 ) ) << "(" << pdg1000130270 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000140280 [ Si28] -----> number-of-initial-interactions = " << pdg1000140280 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000140280 ) ) << "(" << pdg1000140280 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000160320 [  S32] -----> number-of-initial-interactions = " << pdg1000160320 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000160320 ) ) << "(" << pdg1000160320 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000180400 [ Ar40] -----> number-of-initial-interactions = " << pdg1000180400 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000180400 ) ) << "(" << pdg1000180400 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000190390 [  K39] -----> number-of-initial-interactions = " << pdg1000190390 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000190390 ) ) << "(" << pdg1000190390 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000200400 [ Ca40] -----> number-of-initial-interactions = " << pdg1000200400 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000200400 ) ) << "(" << pdg1000200400 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000260560 [ Fe56] -----> number-of-initial-interactions = " << pdg1000260560 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000260560 ) ) << "(" << pdg1000260560 / double( size ) * 100 << "%)" << endl
         << " |---o code: 1000260560 [Gd157] -----> number-of-initial-interactions = " << pdg1000641570 << std::setw( nDigits( max ) + 2 - nDigits( pdg1000641570 ) ) << "(" << pdg1000641570 / double( size ) * 100 << "%)" << endl;
}
