void macro() {
  // Create canvas
  auto canv = new TCanvas("canv", "canv", 4096, 2160);
  canv->SetGrid();

  // Create Graphs with style
  auto graph =
      new TGraph("./data/169.dat", "%lg %lg");
  //   low_graph->SetTitle("high current");
  //   low_graph->SetLineColor(kRed - 2);
  //   low_graph->SetLineWidth(2);
  graph->SetMarkerStyle(20);

  graph->Draw();
  canv->Print("./grafico.png");

  delete canv;
}