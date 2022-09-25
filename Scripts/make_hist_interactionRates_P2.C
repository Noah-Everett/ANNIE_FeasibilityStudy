#include <cmath>

using namespace std;

struct interaction {
    double Ev  ; // neutrino energy
    int    neu ; // neutrino pdg code
    int    tgt ; // target pdg code
    bool   qel ; // quasi-elastic stattering
    bool   cc  ; // charge current
    double vtxx; // x interaction vertex
    double vtxy; // y interaction vertex
    double vtxz; // z interaction vertex
};

double sigma_tot( TChain* tree, TH1F*& hist, double scale, bool verbose = false )
{
    double sigma = 0;
    TTree* cur = nullptr;
    int nFile = 0;
    for( int i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );
        // cout << tree->GetTreeNumber() << endl;
        if( nFile != tree->GetTreeNumber() ) {
            ++nFile;
            cur = tree->GetTree();
            sigma += pow( ( cur->GetEntries() * scale ) - ( hist->GetEntries() * scale / 500.0 ), 2 );
            if( verbose ) cout << "i: " << nFile << " || mean_i: " << cur->GetEntries() * scale << " || mean: " << hist->GetEntries() * scale / 500.0 << " || sigma: " << sigma << endl;
        }
    }

    return sqrt( sigma / 499.0 );
}
double sigma_nu( TChain* tree, TH1F*& hist, double scale, int pdg, bool verbose = false )
{
    double sigma = 0;
    TTree* cur = nullptr;
    int nFile = 0;
    int entries = 0;
    int neu;
    for( int i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );
        // cout << tree->GetTreeNumber() << endl;
        if( nFile != tree->GetTreeNumber() ) {
            ++nFile;
            cur = tree->GetTree();
            cur->SetBranchAddress( "neu", &neu );
            for( int j = 0; j < cur->GetEntries(); j++ ) {
                cur->GetEntry( i );
                if( neu == pdg ) ++entries;
            }
            sigma += pow( ( entries * scale ) - ( hist->GetEntries() * scale / 500.0 ), 2 );
            if( verbose ) cout << "i: " << nFile << " || mean_i: " << cur->GetEntries() * scale << " || mean: " << hist->GetEntries() * scale / 500.0 << " || sigma: " << sigma << endl;
        }
    }

    return sqrt( sigma / 499.0 );
}
double sigma_cc( TChain* tree, TH1F*& hist, double scale, bool verbose = false )
{
    double sigma = 0;
    TTree* cur = nullptr;
    int nFile = 0;
    int entries = 0;
    bool cc;
    for( int i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );
        // cout << tree->GetTreeNumber() << endl;
        if( nFile != tree->GetTreeNumber() ) {
            ++nFile;
            cur = tree->GetTree();
            cur->SetBranchAddress( "cc", &cc );
            for( int j = 0; j < cur->GetEntries(); j++ ) {
                cur->GetEntry( i );
                if( cc ) ++entries;
            }
            sigma += pow( ( entries * scale ) - ( hist->GetEntries() * scale / 500.0 ), 2 );
            if( verbose ) cout << "i: " << nFile << " || mean_i: " << cur->GetEntries() * scale << " || mean: " << hist->GetEntries() * scale / 500.0 << " || sigma: " << sigma << endl;
        }
    }

    return sqrt( sigma / 499.0 );
}
double sigma_qel( TChain* tree, TH1F*& hist, double scale, bool verbose = false )
{
    double sigma = 0;
    TTree* cur = nullptr;
    int nFile = 0;
    int entries = 0;
    bool qel;
    for( int i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );
        // cout << tree->GetTreeNumber() << endl;
        if( nFile != tree->GetTreeNumber() ) {
            ++nFile;
            cur = tree->GetTree();
            cur->SetBranchAddress( "qel", &qel );
            for( int j = 0; j < cur->GetEntries(); j++ ) {
                cur->GetEntry( i );
                if( qel ) ++entries;
            }
            sigma += pow( ( entries * scale ) - ( hist->GetEntries() * scale / 500.0 ), 2 );
            if( verbose ) cout << "i: " << nFile << " || mean_i: " << cur->GetEntries() * scale << " || mean: " << hist->GetEntries() * scale / 500.0 << " || sigma: " << sigma << endl;
        }
    }

    return sqrt( sigma / 499.0 );
}

void make_hist_interactionRates_P2( vector< string > fileNumbers, string fileDirectory )
{   
    gROOT->ForceStyle(); // loads ANNIE plot style if enabled
    gStyle->SetOptStat(0); // dissable histogram's stats box

    // open tree
    TCanvas* canvas = new TCanvas( "c1", "", 1000, 1000 );
    canvas->SetLogy();

    vector< string > fileNames;
    for( int i = 0; i < fileNumbers.size(); i++ )
        fileNames.push_back( fileDirectory + "/gntp." + fileNumbers[ i ] + ".gst.root" );
    // vector< TFile* > files;
    // vector< TTree* > trees;
    // for( int i = 0; i < fileNumbers.size(); i++ ) {
    //     files.push_back( TFile::Open( fileNames[ i ].c_str() ) );
    //     trees.push_back( ( TTree* )files[ i ]->Get( "gst;1" ) );
    // }
    TChain* tree = new TChain( "gst;1" );
    for( string fileName : fileNames )
        tree->Add( fileName.c_str() );
    
    // pdg codes
    int pdg_nu_e        =  12;
    int pdg_nu_e_anti   = -12;
    int pdg_nu_mu       =  14;
    int pdg_nu_mu_anti  = -14;
    int pdg_nu_tau      =  16;
    int pdg_nu_tau_anti = -16;

    // interaction info objects
    vector< interaction > interactions;
    interaction temp; // used to fill interactions

    // Used to determine hist limits
    double max_Ev  = -1;
    double min_Ev  = -1;

    tree->SetBranchAddress( "Ev"  , &temp.Ev   );
    tree->SetBranchAddress( "neu" , &temp.neu  );
    tree->SetBranchAddress( "tgt" , &temp.tgt  );
    tree->SetBranchAddress( "qel" , &temp.qel  );
    tree->SetBranchAddress( "cc"  , &temp.cc   );
    tree->SetBranchAddress( "vtxx", &temp.vtxx );
    tree->SetBranchAddress( "vtxy", &temp.vtxy );
    tree->SetBranchAddress( "vtxz", &temp.vtxz );

    for( Int_t i = 0; i < tree->GetEntries(); i++ ) {
        tree->GetEntry( i );

        temp.Ev *= 1000; // GeV --> MeV

        if( max_Ev == -1 ) {
            max_Ev  = temp.Ev ;
            min_Ev  = temp.Ev ;
        }
        if( temp.Ev  > max_Ev  ) max_Ev  = temp.Ev ;
        if( temp.Ev  < min_Ev  ) min_Ev  = temp.Ev ;

        interactions.push_back( temp );
    } 

    int nBins_GeV = 20; // number of bins per GeV
    int nBins_x = ceil( max_Ev * nBins_GeV / 1000 ) - floor( min_Ev * nBins_GeV / 1000);

    // Total events
    TH1F* hist_tot = new TH1F( "", "Simulated interactions in ANNIE;" "E_{#nu} [MeV];" "Events / 50 MeV / 5E12 POT;", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) hist_tot->Fill( interactions[ i ].Ev );
    hist_tot->GetXaxis()->SetTitleOffset( 1.1 );
    hist_tot->GetYaxis()->SetTitleOffset( 1.5 ); // 1.0 (linear), 1.4 (log)
    
    // nu_e events
    TH1F* hist_nu_e = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_e ) hist_nu_e->Fill( interactions[ i ].Ev );
    
    // nu_-e events
    TH1F* hist_nu_e_anti = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_e_anti ) hist_nu_e_anti->Fill( interactions[ i ].Ev );
    
    // nu_mu events
    TH1F* hist_nu_mu = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_mu ) hist_nu_mu->Fill( interactions[ i ].Ev );
    
    // nu_-mu events
    TH1F* hist_nu_mu_anti = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_mu_anti ) hist_nu_mu_anti->Fill( interactions[ i ].Ev );
    
    // nu_tau events
    TH1F* hist_nu_tau = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_tau ) hist_nu_tau->Fill( interactions[ i ].Ev );
    
    // nu_-tau events
    TH1F* hist_nu_tau_anti = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].neu == pdg_nu_tau ) hist_nu_tau_anti->Fill( interactions[ i ].Ev );
    
    // CC events
    TH1F* hist_cc = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].cc && interactions[ i ].neu == pdg_nu_mu ) hist_cc->Fill( interactions[ i ].Ev );
    
    // qel events
    TH1F* hist_qel = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].qel && interactions[ i ].neu == pdg_nu_mu ) hist_qel->Fill( interactions[ i ].Ev );
    
    // // CCQE in fiducial volume
    // TH1F* hist_yes = new TH1F( "", "", nBins_x, floor( min_Ev ), ceil( max_Ev ) );
    // for( int i = 0; i < interactions.size(); i++ ) if( interactions[ i ].qel && interactions[ i ].cc && interactions[ i ].neu == pdg_nu_mu ) hist_yes->Fill( interactions[ i ].Ev );
    
    hist_tot        ->SetLineColor( kBlack );
    hist_nu_e       ->SetLineColor( kBlack );
    hist_nu_e_anti  ->SetLineColor( kBlack );
    hist_nu_mu      ->SetLineColor( kBlack );
    hist_nu_mu_anti ->SetLineColor( kBlack );
    hist_nu_tau     ->SetLineColor( kBlack );
    hist_nu_tau_anti->SetLineColor( kBlack );
    hist_cc         ->SetLineColor( kBlack );
    hist_qel        ->SetLineColor( kBlack );
    hist_tot        ->SetLineWidth( 1 );
    hist_nu_e       ->SetLineWidth( 1 );
    hist_nu_e_anti  ->SetLineWidth( 1 );
    hist_nu_mu      ->SetLineWidth( 1 );
    hist_nu_mu_anti ->SetLineWidth( 1 );
    hist_nu_tau     ->SetLineWidth( 1 );
    hist_nu_tau_anti->SetLineWidth( 1 );
    hist_cc         ->SetLineWidth( 1 );
    hist_qel        ->SetLineWidth( 1 );
    hist_tot        ->SetFillColorAlpha( kBlack   , 0.2 );
    hist_nu_e       ->SetFillColorAlpha( kBlue-9  , 0.6 );
    hist_nu_e_anti  ->SetFillColorAlpha( kCyan-3  , 1.0 );
    hist_nu_mu      ->SetFillColorAlpha( kRed+2   , 1.0 );
    hist_nu_mu_anti ->SetFillColorAlpha( kRed-7   , 1.0 );
    hist_nu_tau     ->SetFillColorAlpha( kGreen+2 , 1.0 );
    hist_nu_tau_anti->SetFillColorAlpha( kGreen-6 , 1.0 );
    hist_cc         ->SetFillColorAlpha( kOrange-7, 1.0 );
    hist_qel        ->SetFillColorAlpha( kOrange-2, 1.0 );

    string label_tot         = "Total events in ANNIE"; label_tot         += " (" + to_string( int( hist_tot        ->GetEntries() ) ) + " events)";
    string label_nu_e        = "#nu_{e}"              ; label_nu_e        += " (" + to_string( int( hist_nu_e       ->GetEntries() ) ) + " events)";
    string label_nu_e_anti   = "#bar{#nu}_{e}"        ; label_nu_e_anti   += " (" + to_string( int( hist_nu_e_anti  ->GetEntries() ) ) + " events)";
    string label_nu_mu       = "#nu_{#mu}"            ; label_nu_mu       += " (" + to_string( int( hist_nu_mu      ->GetEntries() ) ) + " events)";
    string label_nu_mu_anti  = "#bar{#nu}_{#mu}"      ; label_nu_mu_anti  += " (" + to_string( int( hist_nu_mu_anti ->GetEntries() ) ) + " events)";
    string label_nu_tau      = "#nu_{#tau}"           ; label_nu_tau      += " (" + to_string( int( hist_nu_tau     ->GetEntries() ) ) + " events)";
    string label_nu_tau_anti = "#bar{#nu}_{#tau}"     ; label_nu_tau_anti += " (" + to_string( int( hist_nu_tau_anti->GetEntries() ) ) + " events)";
    string label_cc          = "CC events"            ; label_cc          += " (" + to_string( int( hist_cc         ->GetEntries() ) ) + " events)";
    string label_qel         = "QE events"            ; label_qel         += " (" + to_string( int( hist_qel        ->GetEntries() ) ) + " events)";

    TLegend* legend = new TLegend(0.442693,0.606316,0.863897,0.886216);
    legend->AddEntry( hist_tot        , label_tot        .c_str() );
    legend->AddEntry( hist_nu_e       , label_nu_e       .c_str() );
    legend->AddEntry( hist_nu_e_anti  , label_nu_e_anti  .c_str() );
    legend->AddEntry( hist_nu_mu      , label_nu_mu      .c_str() );
    legend->AddEntry( hist_nu_mu_anti , label_nu_mu_anti .c_str() );
    legend->AddEntry( hist_nu_tau     , label_nu_tau     .c_str() );
    legend->AddEntry( hist_nu_tau_anti, label_nu_tau_anti.c_str() );
    legend->AddEntry( hist_cc         , label_cc         .c_str() );
    legend->AddEntry( hist_qel        , label_qel        .c_str() );
    legend->SetBorderSize( 0 );

    double scale = double( 1E12 ) / double( 1E18 * 500 );
    double norm  = hist_tot->GetEntries() * scale;
    cout << "Scale: " << scale << endl;

    cout << "Entries: hist_tot        : " << setw( 6 ) << hist_tot        ->GetEntries() << " || Scaled Entries: hist_tot        : " << hist_tot        ->GetEntries() * scale << endl;
    cout << "Entries: hist_nu_mu      : " << setw( 6 ) << hist_nu_mu      ->GetEntries() << " || Scaled Entries: hist_nu_mu      : " << hist_nu_mu      ->GetEntries() * scale << endl;
    cout << "Entries: hist_cc         : " << setw( 6 ) << hist_cc         ->GetEntries() << " || Scaled Entries: hist_cc         : " << hist_cc         ->GetEntries() * scale << endl;
    cout << "Entries: hist_qel        : " << setw( 6 ) << hist_qel        ->GetEntries() << " || Scaled Entries: hist_qel        : " << hist_qel        ->GetEntries() * scale << endl;
    cout << "Entries: hist_nu_mu_anti : " << setw( 6 ) << hist_nu_mu_anti ->GetEntries() << " || Scaled Entries: hist_nu_mu_anti : " << hist_nu_mu_anti ->GetEntries() * scale << endl;
    cout << "Entries: hist_nu_e       : " << setw( 6 ) << hist_nu_e       ->GetEntries() << " || Scaled Entries: hist_nu_e       : " << hist_nu_e       ->GetEntries() * scale << endl;
    cout << "Entries: hist_nu_e_anti  : " << setw( 6 ) << hist_nu_e_anti  ->GetEntries() << " || Scaled Entries: hist_nu_e_anti  : " << hist_nu_e_anti  ->GetEntries() * scale << endl;
    cout << "Entries: hist_nu_tau_anti: " << setw( 6 ) << hist_nu_tau_anti->GetEntries() << " || Scaled Entries: hist_nu_tau_anti: " << hist_nu_tau_anti->GetEntries() * scale << endl;
    cout << "Entries: hist_nu_tau     : " << setw( 6 ) << hist_nu_tau     ->GetEntries() << " || Scaled Entries: hist_nu_tau     : " << hist_nu_tau     ->GetEntries() * scale << endl;
   
    // cout << "hist_tot         --> MEAN:" << setw( 6 ) << hist_tot        ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_tot( tree, hist_tot        , 1.0                  ) << " || MEAN (1E12): " << setw( 12 ) << hist_tot        ->GetEntries() * scale << endl;
    // cout << "hist_nu_mu       --> MEAN:" << setw( 6 ) << hist_nu_mu      ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_nu ( tree, hist_nu_mu      , 1.0, pdg_nu_mu       ) << " || MEAN (1E12): " << setw( 12 ) << hist_nu_mu      ->GetEntries() * scale << endl;
    // cout << "hist_cc          --> MEAN:" << setw( 6 ) << hist_cc         ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_cc ( tree, hist_cc         , 1.0                  ) << " || MEAN (1E12): " << setw( 12 ) << hist_cc         ->GetEntries() * scale << endl;
    // cout << "hist_qel         --> MEAN:" << setw( 6 ) << hist_qel        ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_qel( tree, hist_qel        , 1.0                  ) << " || MEAN (1E12): " << setw( 12 ) << hist_qel        ->GetEntries() * scale << endl;
    // cout << "hist_nu_mu_anti  --> MEAN:" << setw( 6 ) << hist_nu_mu_anti ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_nu ( tree, hist_nu_mu_anti , 1.0, pdg_nu_mu_anti  ) << " || MEAN (1E12): " << setw( 12 ) << hist_nu_mu_anti ->GetEntries() * scale << endl;
    // cout << "hist_nu_e        --> MEAN:" << setw( 6 ) << hist_nu_e       ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_nu ( tree, hist_nu_e       , 1.0, pdg_nu_e        ) << " || MEAN (1E12): " << setw( 12 ) << hist_nu_e       ->GetEntries() * scale << endl;
    // cout << "hist_nu_e_anti   --> MEAN:" << setw( 6 ) << hist_nu_e_anti  ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_nu ( tree, hist_nu_e_anti  , 1.0, pdg_nu_e_anti   ) << " || MEAN (1E12): " << setw( 12 ) << hist_nu_e_anti  ->GetEntries() * scale << endl;
    // cout << "hist_nu_tau_anti --> MEAN:" << setw( 6 ) << hist_nu_tau_anti->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_nu ( tree, hist_nu_tau_anti, 1.0, pdg_nu_tau_anti ) << " || MEAN (1E12): " << setw( 12 ) << hist_nu_tau_anti->GetEntries() * scale << endl;
    // cout << "hist_nu_tau      --> MEAN:" << setw( 6 ) << hist_nu_tau     ->GetEntries() << " || SIGMA: " << setw( 8 ) << sigma_nu ( tree, hist_nu_tau     , 1.0, pdg_nu_tau      ) << " || MEAN (1E12): " << setw( 12 ) << hist_nu_tau     ->GetEntries() * scale << endl;

    hist_tot        ->Scale( scale );
    hist_nu_e       ->Scale( scale );
    hist_nu_e_anti  ->Scale( scale );
    hist_nu_mu      ->Scale( scale );
    hist_nu_mu_anti ->Scale( scale );
    hist_nu_tau     ->Scale( scale );
    hist_nu_tau_anti->Scale( scale );
    hist_cc         ->Scale( scale );
    hist_qel        ->Scale( scale );
    
    hist_tot        ->SetMaximum  ( 1.5E-03  );
    hist_nu_e       ->SetMaximum  ( 1.5E-03  );
    hist_nu_e_anti  ->SetMaximum  ( 1.5E-03  );
    hist_nu_mu      ->SetMaximum  ( 1.5E-03  );
    hist_nu_mu_anti ->SetMaximum  ( 1.5E-03  );
    hist_nu_tau     ->SetMaximum  ( 1.5E-03  );
    hist_nu_tau_anti->SetMaximum  ( 1.5E-03  );
    hist_cc         ->SetMaximum  ( 1.5E-03  );
    hist_qel        ->SetMaximum  ( 1.5E-03  );

    hist_tot        ->Draw( "HIST"      );
    hist_nu_mu      ->Draw( "same HIST" );
    hist_cc         ->Draw( "same HIST" );
    hist_qel        ->Draw( "same HIST" );
    hist_nu_mu_anti ->Draw( "same HIST" );
    hist_nu_e       ->Draw( "same HIST" );
    hist_nu_e_anti  ->Draw( "same HIST" );
    hist_nu_tau     ->Draw( "same HIST" );
    hist_nu_tau_anti->Draw( "same HIST" );
    legend          ->Draw( "Same"      );

    gPad->RedrawAxis();

    canvas->SaveAs("interactionRates.pdf");
}
