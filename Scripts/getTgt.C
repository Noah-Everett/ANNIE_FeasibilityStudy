using std::map;
using std::string;

struct info {
    string name;
    string pdg;
    int events{ 0 };
    map< string, int > location{ { "TWATER_LV", 0 }, { "TDEWAR_LV", 0 }, { "TARGON_LV", 0 } };
};


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

void getTgt( std::string nRun, bool locations = false, string geomLocation = "", bool verbose = false ) 
{
    std::string fileName = "gntp." + nRun + ".gst.root";
    
    TFile* file = TFile::Open( fileName.c_str() );
    TTree* tree = ( TTree* )file->Get( "gst;1" );
    
    const unsigned int size = tree->GetEntries();
    int tgt_cur;
    TGeoManager* geom;
    if( locations )
        geom = TGeoManager::Import( geomLocation.c_str() );
    TGeoNode*    node;
    TGeoVolume*  vol;
    string location;
    Double_t x_cur, y_cur, z_cur;

    tree->SetBranchAddress( "tgt",  &tgt_cur );
    tree->SetBranchAddress( "vtxx", &x_cur   );
    tree->SetBranchAddress( "vtxy", &y_cur   );
    tree->SetBranchAddress( "vtxz", &z_cur   );
    
    map< int, info > materials = { { 11        , { "   e-", "11        " } }, { 2212      , { "    p", "2212      " } }, { 1000010010, { "   H1", "1000010010" } },
                                   { 1000060120, { "  C12", "1000060120" } }, { 1000070140, { "  N14", "1000070140" } }, { 1000080160, { "  O16", "1000080160" } },
                                   { 1000110230, { " Na23", "1000110230" } }, { 1000120240, { " Mg24", "1000120240" } }, { 1000130270, { " Al27", "1000130270" } },
                                   { 1000140280, { " Si28", "1000140280" } }, { 1000160320, { "  S32", "1000160320" } }, { 1000180400, { " Ar40", "1000180400" } },
                                   { 1000190390, { "  K39", "1000190390" } }, { 1000200400, { " Ca40", "1000200400" } }, { 1000260560, { " Fe56", "1000260560" } },
                                   { 1000641570, { "Gd157", "1000641570" } } };

    for( Int_t i = 0; i < size; i++ ) {
        tree->GetEntry( i );

        // if( materials.contains( tgt_cur ) )
        //     ++materials.at( tgt_cur ).events;
        // else
        //     cout << "Unknown pdg: " << tgt_cur << endl;
        
        if( verbose )
            cout << "pdg code: " << tgt_cur << endl; // Verbose
        ++materials.at( tgt_cur ).events;
        if( locations ) {
            node = geom->FindNode( x_cur *= 100, y_cur *= 100, z_cur *= 100 );
            vol  = node->GetVolume();
            location = vol->GetName();
            // if( materials.at( tgt_cur ).location.contains( location ) )
                // ++materials.at( tgt_cur ).location.at( location );
            // else
                if( verbose )
                    cout << "(" << x_cur << ", " << y_cur << ", " << z_cur << "), volume: " << location << endl;
                ++materials.at( tgt_cur ).location.at( location );
        }
    }

    int max = 0;
    for( const auto& i : materials )
        if( i.second.events > max ) max = i.second.events;

    int max_sub = 0;
    cout << "[-]" << endl;
    for( const auto& i : materials ) {
        if( i.second.events != 0 ) {
            for( const auto& j : i.second.location )
                if( j.second > max_sub ) max_sub = j.second;
            cout << " |---o code: " << i.second.pdg << " [" << i.second.name << "] o----> total-number-of-initial-interactions = " << i.second.events << std::setw( nDigits( max ) + 2 - nDigits( i.second.events ) ) << "(" << i.second.events / double( size ) * 100 << "%)" << endl;
            for( const auto& j : i.second.location )
                if( j.second != 0 )
                    cout << " |                              |--> number-of-initial-interactions-in-" << j.first <<  " = " << j.second << std::setw( nDigits( max_sub ) + 2 - nDigits( j.second ) ) << " (" << j.second / double( i.second.events ) * 100 << "%)" << endl;
            if( locations ) cout << " |" << endl;
        }
        max_sub = 0;
    }
}
