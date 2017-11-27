/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;





/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println(urlVal.isValid("http://www.amazon.com"));
	   System.out.println(urlVal.isValid("www.amazon.com"));
	   System.out.println(urlVal.isValid("f3tp://www.amazon.com"));
	   System.out.println(urlVal.isValid("3ttp://www.amazon.com"));
	   System.out.println(urlVal.isValid("http://google.com/search?query=some+query"));
	   System.out.println(urlVal.isValid("http://google.com/search"));
	   System.out.println(urlVal.isValid("http://google.com/search?query=some"));
	   System.out.println(urlVal.isValid("http://google.com/search?query"));
	   
   }
   
   
   public void testYourFirstPartition()
   {
       UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
       System.out.println("-------- TEST PARTITION:  SCHEME ----------");

       System.out.println("Valid Schemes (all values should be true: ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com") + " -- http://www.amazon.com");
       System.out.println("\t" + urlVal.isValid("https://www.amazon.com") + " -- https://www.amazon.com");
       System.out.println("\t" + urlVal.isValid("ftp://www.amazon.com") + " -- ftp://www.amazon.com");

       System.out.println("Invalid Schemes (all values should be false: ");
       System.out.println("\t" + urlVal.isValid("http:///www.amazon.com") + " -- http:///www.amazon.com");
       System.out.println("\t" + urlVal.isValid("https//:www.amazon.com") + " -- http//:www.amazon.com");
       System.out.println("\t" + urlVal.isValid("http:www.amazon.com") + " -- http:www.amazon.com");
       System.out.println("\t" + urlVal.isValid("http//www.amazon.com") + " -- http//www.amazon.com");
   }
   
   public void testYourSecondPartition()
   {
       UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
       System.out.println("-------- TEST PARTITION:  HOST ----------");

       System.out.println("Valid Hosts (all values should be true): ");
       System.out.println("\t" + urlVal.isValid("http://amazon.com") + " -- http://amazon.com");
       System.out.println("\t" + urlVal.isValid("http://www.ama-zon.com") + " -- http://www.ama-zon.com");
       System.out.println("\t" + urlVal.isValid("http://www.2amazon.com") + " -- http://www.2amazon.com");
       System.out.println("\t" + urlVal.isValid("http://ww2.amazon.com") + " -- http://ww2.amazon.com");
       System.out.println("\t" + urlVal.isValid("http://128.193.40.35") + " -- http://128.193.40.35");

       System.out.println("Invalid Schemes (all values should be false): ");
       System.out.println("\t" + urlVal.isValid("http://www.@mazon.com") + " -- http://www.@mazon.com");
       System.out.println("\t" + urlVal.isValid("https://www.ama zon.com") + " -- http://www.ama zon.com");
       System.out.println("\t" + urlVal.isValid("http://128.193.40.256") + " -- http://128.193.40.256");
   }
   
	
   public void testOtherPartitions()
   {
       UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
       System.out.println("-------- TEST PARTITION:  TLD ----------");
       System.out.println("Valid TLD (value should be true): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com") + " -- http://www.amazon.com");
       System.out.println("Invalid TLD (value should be false): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.ak") + " -- http://www.amazon.ak");

       System.out.println("-------- TEST PARTITION:  PORT----------");
       System.out.println("Valid TLD (value should be true): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com:80") + " -- http://www.amazon.com:36");
       System.out.println("Invalid TLD (value should be false): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com:655300") + " -- http://www.amazon.com:65530");

       System.out.println("-------- TEST PARTITION:  PATH----------");
       System.out.println("Valid TLD (value should be true): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com/somepath") + " -- http://www.amazon.com/somepath");
       System.out.println("Invalid TLD (value should be false): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com/some path") + " -- http://www.amazon.com/some path");
       
       System.out.println("-------- TEST PARTITION:  QUERY----------");
       System.out.println("Valid TLD (value should be true): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com/somepath?somequery") + " -- http://www.amazon.com/somepath?somequery");
       System.out.println("Invalid TLD (value should be false): ");
       System.out.println("\t" + urlVal.isValid("http://www.amazon.com/somepath?some query") + " -- http://www.amazon.com/somepath?some query");
       
   }
	
   public void testIsValid()
   {
       UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
       String testUrl = null;
       boolean testValidity = true; 
       System.out.println("\nProgramming-based testing: testIsValid()");
       
       //loop through all permutations of test case
       for (int a = 0; a < testUrlScheme.length; a++) {
    	   		for (int b = 0; b < testUrlScheme[a].length; b++) 
    	   			for(int c = 0; c < testHost.length; c++) {
    	   				for(int d = 0; d < testHost[c].length; d++) {
    	   					for(int e = 0; e < testTLD.length; e++) {
    	   						for(int f = 0; f < testTLD[e].length; f++) {
    	   							for(int g = 0; g < testPort.length; g++) {
    	   								for(int h = 0; h < testPort[g].length; h++) {
    	   									for(int i = 0; i < testPath.length; i++) {
    	   										for(int j = 0; j < testPath[i].length; j++) {
    	   											for(int k = 0; k < testQuery.length; k++) {
    	   												for(int l = 0; l < testQuery[k].length; l++) {
    	   													//concatenate our testUrl 
    	   													testUrl = testUrlScheme[a][b] + testHost[c][d] + testTLD[e][f] + testPort[g][h] + testPath[i][j] + testQuery[k][l];
    	   													//all valid test cases are stored in the array at the 0 index of their respective arrays
    	   													if(a == 0 && c == 0 && e == 0 && g == 0 && i == 0 && k == 0) { 
    	   														testValidity = true; 
    	   													}
    	   													else {
    	   														testValidity = false; 
    	   													}
    	   													//print out results if reveal an unexpected result, ie. a likely bug. 
    	   													if(urlVal.isValid(testUrl) != testValidity) {
    	   										   	   			System.out.println("\t"+testUrl);
    	   										   	   			System.out.println("\t\t" + "Expected result: " + testValidity + ", Actual result: " + urlVal.isValid(testUrl));
    	   													}
    	   												}
    	   											}
    	   										}
    	   									}
    	   								}
    	   							}
    	   						}
    	   					}
    	   				}
    	   			}
    	   		}
       }
   
   public void testAnyOtherUnitTest()
   {
       UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
       String site = "http://www.amazon.com";
       System.out.println("Other Unit Test: change one input at a time");
       String testUrl = null;

	   for(int i = 0; i < allPartitions.length; i++) {
		   
		   if(allPartitions[i] == validPartitions) {
			   System.out.println("Testing Valid Partitions (all should be true)");
		   }
		   else {
			   System.out.println("Testing Invalid Partitions (all should be false)");
		   }
	       for(int j = 0; j < allPartitions[i].length; j++) {
	   			if(allPartitions[i][j] == testUrlSchemeValid || allPartitions[i][j] == testUrlSchemeInvalid) {
       	   			System.out.println("\tTesting Scheme");
   	   			}
   	   			else if(allPartitions[i][j] == testHostValid || allPartitions[i][j] == testHostInvalid) {
       	   			System.out.println("\tTesting Host");
   	   			}
   	   			else if(allPartitions[i][j] == testTLDValid || allPartitions[i][j] == testTLDInvalid) {
       	   			System.out.println("\tTesting Top-Level Domain");
   	   			}
   	   			else if(allPartitions[i][j] == testPortValid || allPartitions[i][j] == testPortInvalid) {
       	   			System.out.println("\tTesting Port");
   	   			}
   	   			else if(allPartitions[i][j] == testPathValid || allPartitions[i][j] == testPathInvalid) {
       	   			System.out.println("\tTesting Path");
   	   			}
   	   			else if(allPartitions[i][j] == testQueryValid || allPartitions[i][j] == testQueryInvalid) {
       	   			System.out.println("\tTesting Query");
   	   			}
   	   		for(int k = 0; k < allPartitions[i][j].length; k++){
   			  
   	   			if(allPartitions[i][j] == testUrlSchemeValid || allPartitions[i][j] == testUrlSchemeInvalid) {
       	   			testUrl = site.replaceAll("(^[a-z]+://)", allPartitions[i][j][k]);
   	   			}
   	   			else if(allPartitions[i][j] == testHostValid || allPartitions[i][j] == testHostInvalid) {
       	   			testUrl = site.replaceAll("www.amazon", allPartitions[i][j][k]);
   	   			}
   	   			else if(allPartitions[i][j] == testTLDValid || allPartitions[i][j] == testTLDInvalid) {
       	   			testUrl = site.replaceAll(".com", allPartitions[i][j][k]);
   	   			}
   	   			else if(allPartitions[i][j] == testPortValid || allPartitions[i][j] == testPortInvalid) {
       	   			testUrl = site + allPartitions[i][j][k];
   	   			}
   	   			else if(allPartitions[i][j] == testPathValid || allPartitions[i][j] == testPathInvalid) {
       	   			testUrl = site + allPartitions[i][j][k];
   	   			}
   	   			else if(allPartitions[i][j] == testQueryValid || allPartitions[i][j] == testQueryInvalid) {
       	   			testUrl = site + allPartitions[i][j][k];
   	   			}
   	   			System.out.println("\t\t"+testUrl + " - " + urlVal.isValid(testUrl));
   	   		 }
	      }
	   }
   }
   
   /**
    * Create set of tests by taking the testUrlXXX arrays and
    * running through all possible permutations of their combinations.
    *
    * @param testObjects Used to create a url.
    */
 
   	String testUrlSchemeValid[] = {"http://", "https://", "ftp://", "h3t://"};
   	String testUrlSchemeInvalid[] = {"3ht://", "http:/", "http:///", ":"};
   	
   	String testHostValid[] = {"www.google", "3com", "google", "ama-zon", "www2.reddit", "www3.google", "en.wikipedia"};
   	String testHostInvalid[] = {"am3_zon", "www.ama zon", "ama zon", "amazon ", " amazon", "@mazon", "www3.red;dit", "-www.oregonstate", "-oregonstate"};
   	
   	String testTLDValid[] = {".com", ".co.uk", ".net", ".biz", ".org", ".edu", ".gov", ".ac", ".ba", ".bb", ".cf", ".de", ".ec", ".hk", ".ma", ".pa", ".sb", ".tc", ".va", ".wf", ".ye", ".za"};
    String testTLDInvalid[] = {".ak", ".ik", "com", "net", "biz", ".zb"};
    
    String testPortValid[] = {"" , ":0", ":80", ":32217", ":65535"};
    String testPortInvalid[] = {":-1", ":65536", ":610234", ":80x", ":j80"};
    
    String testPathValid[] = {"", "/index1", "/index123", "/index/file/test", "/$78", "/index.html"};
    String testPathInvalid[] = {"/..", "index//file", "/../file1"};
    
    String testQueryValid[] = {"", "?id=100", "?mode=test&result=unknown", "?key1=value1;key2=value2"};
    String testQueryInvalid[] = {"?id+100", "?mode+test&result_unknown"};
   	
    String validPartitions[][] = {testUrlSchemeValid, testHostValid, testTLDValid, testPortValid, testPathValid, testQueryValid};
    String invalidPartitions[][] = {testUrlSchemeInvalid, testHostInvalid, testTLDInvalid, testPortInvalid, testPathInvalid, testQueryInvalid};
    
    String allPartitions[][][] = {validPartitions, invalidPartitions};
    
    String testUrlScheme[][] = {testUrlSchemeValid, testUrlSchemeInvalid};
    String testHost[][] = {testHostValid, testHostInvalid};
    String testTLD[][] = {testTLDValid, testTLDInvalid};
    String testPort[][] = {testPortValid, testPortInvalid};
    String testPath[][] = {testPathValid, testPathInvalid};
    String testQuery[][] = {testQueryValid, testQueryInvalid};
    
}

