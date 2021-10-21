/*
 * Copyright (C) EB Automotive
 * All rights reserved
 */

package dreisoft.tresos.demo.resource.examples;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.datamodel2.api.model.Location;
import dreisoft.tresos.datamodel2.api.model.RCtxt;
import dreisoft.tresos.demo.resource.DemoBackend;
import dreisoft.tresos.demo.resource.DemoPlugin;
import dreisoft.tresos.demo.resource.generated.nls.ResourceAPIDemoNLS;
import dreisoft.tresos.demo.resource.generated.operationstatus.ResourceAPIDemoOperationStatus;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Example using the Resource API for merging two module configuration files and saving the result to another file.
 *
 * @since 11
 */
public class MergeExample extends ExampleBase
{

    /**
     * @param backend the demo backend that uses the example
     * @since 11
     */
    public MergeExample(DemoBackend backend)
    {
        super(backend, ResourceAPIDemoNLS.DESCRIPTION_MERGE_EXAMPLE());
    }

    /**
     * The first file into which the second one gets merged.
     */
    private static final String INFILE_TWO = "resources/mergeExample/ModuleConfigTwo.xdm";
    /**
     * The second file which gets merged into the first one.
     */
    private static final String INFILE_ONE = "resources/mergeExample/ModuleConfigOne.xdm";
    /**
     * The output file (relative to the current project).
     */
    private static final String OUTFILE = "demoOutput/MergedModuleConfig.xdm";

    /**
     * Merges the given input files into the given output file using the public Resource API.
     *
     * @param inFile1 merged to
     * @param inFile2 merged from
     * @param outFileUri merged output
     * @param status where status information is added
     * @since 11
     */
    private static void doMerge(File inFile1, File inFile2, String outFileUri, APIOperationStatus status)
    {
        // create locations from the (XDM) file handles with content auto detection
        Location inLocation1 = new Location(inFile1);
        Location inLocation2 = new Location(inFile2);

        // create a new resource data model context node based on the first file (empty and thus invalid)
        RCtxt merged = RCtxt.create(inLocation1);

        // load the content
        merged.load(null, null, status);

        // create and load
        RCtxt source = RCtxt.create(inLocation2);
        source.load(null, null, status);

        // apply IMPORT_INFO tags by using merge options parameter
        Map<String, String> mergeOptions = new HashMap<String, String>(1);
        mergeOptions.put(DCtxt.MERGE_SOURCE, "MergedFromSomeSource");

        // do the actual merge
        merged.merge(source, mergeOptions, status);

        // set the new location on the result
        merged.setLocation(new Location(outFileUri));

        // store the result to it's new location
        merged.save(null, null, status);
    }

    /**
     * Run the merge example
     *
     * @param result
     * @throws Exception on any error
     *
     * @since 11
     */
    @Override
    public void run(ArrayList<APIOperationStatus> result) throws Exception
    {
        // prepare the in file handles
        File inFile1 = DemoPlugin.getFileInBundle(INFILE_ONE);
        File inFile2 = DemoPlugin.getFileInBundle(INFILE_TWO);

        // prepare the output file URI string
        String outFileUri = getDemoBackend().getOutFileURI(OUTFILE);

        APIOperationStatus status = APIOperationStatus.getOkStatus();

        ///////////////////////////////
        // RESOURCE API IS USED HEREIN
        ///////////////////////////////
        doMerge(inFile1, inFile2, outFileUri, status);

        if (status.isOk())
        {
            // append some trace to be shown in the Results view
            result.add(ResourceAPIDemoOperationStatus.SEE_OUTPUT(outFileUri));

            // make results visible
            getDemoBackend().refreshECUConfigProject();
            getDemoBackend().openEditor(OUTFILE);
        }

        if (!status.isAbsolutelyOk())
        {
            result.add(status);
        }
    }

}
