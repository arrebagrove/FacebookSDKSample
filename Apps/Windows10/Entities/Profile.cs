using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Windows10.Entities
{
    public class Profile
    {
        public static Profile FromJson(string jsonText)
        {
            Profile profile = JsonConvert.DeserializeObject<Profile>(jsonText);
            return profile;
        }

        public string Name { get; set; }

        public string Id { get; set; }
    }
}
